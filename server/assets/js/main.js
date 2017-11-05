require('babel-polyfill')

const Vue = require('../../node_modules/vue/dist/vue.common.js')

Vue.component('meters', {
  props: ['meters'],
  methods: {
    type: function (meter) {
      return meter.type.substr(0, 1).toUpperCase() + meter.type.substr(1)
    }
  },
  template:
`<div>
  <div v-for="meter of meters" :key="meter.id">
    <h3>{{ type(meter) }}: <code>{{ meter.id }}</code></h3>
    <meter-readings :meter="meter"></meter-readings>
  </div>
</div>`
})

Vue.component('meter-readings', {
  props: ['meter'],
  computed: {
    options: function () {
      let vAxisTitle
      let barColor
      switch (this.meter.type) {
        case 'electricity':
          barColor = '#FF851B'
          vAxisTitle = 'Electricity consumption (W)'
          break
        case 'gas':
          barColor = '#0074D9'
          vAxisTitle = 'Gas consumption (m³/h)'
          break
      }
      const options = {
        title: this.meter.id,
        width: 900,
        height: 300,
        bar: {
          groupWidth: '100%'
        },
        colors: [barColor],
        hAxis: {
          title: 'Time',
          format: 'EEE\nd MMM'
        },
        vAxis: {
          title: vAxisTitle
        },
        legend: {
          position: 'none'
        }
      }
      return options
    },
    yColumn: function () {
      switch (this.meter.type) {
        case 'electricity':
          return 'currentConsumptionW'
        case 'gas':
          return 'currentConsumptionM3PerH'
      }
    }
  },
  template: '<column-chart :data="meter.readings" x-column="centerTimestamp" :y-column="yColumn" :options="options"></column-chart>'
})

Vue.component('column-chart', googleChartsComponent({
  props: ['data', 'x-column', 'y-column', 'options'],
  template: '<div></div>',
  mounted: function () {
    const dataTable = google.visualization.arrayToDataTable(this.data)
    const dataView = new google.visualization.DataView(dataTable)
    function getColumnIndex (label) {
      const n = dataView.getNumberOfColumns()
      for (let i = 0; i < n; i++) {
        if (dataView.getColumnLabel(i) === label) {
          return i
        }
      }
      return -1
    }
    dataView.setColumns([this.xColumn, this.yColumn].map(getColumnIndex))

    const chart = new google.visualization.ColumnChart(this.$el)
    chart.draw(dataView, this.options)
  }
}))

let googleChartsState = 'unloaded'
let googleChartsComponentResolvers = []
function googleChartsComponent (component) {
  return function (resolve, reject) {
    switch (googleChartsState) {
      case 'unloaded':
        google.charts.load('current', {'packages': ['corechart']})
        google.charts.setOnLoadCallback(function () {
          googleChartsState = 'loaded'
          for (const resolver of googleChartsComponentResolvers) {
            resolver()
          }
          googleChartsComponentResolvers = []
        })
        googleChartsState = 'loading'
        // Fallthrough!
      case 'loading':
        googleChartsComponentResolvers.push(function () { resolve(component) })
        break
      case 'loaded':
        resolve(component)
        break
    }
  }
}

function addDeltaColumn (readings, totalKeys, deltaKey, timeUnitSeconds) {
  const timestampIndex = readings[0].indexOf('timestamp')
  const totalIndices = totalKeys.map(totalKey => readings[0].indexOf(totalKey))
  readings[0].push('centerTimestamp')
  readings[0].push(deltaKey)
  let previousRow = readings[1]
  previousRow.push(NaN)
  previousRow.push(NaN)
  for (let i = 2; i < readings.length; i++) {
    const currentRow = readings[i]
    const deltaTime = currentRow[timestampIndex] - previousRow[timestampIndex]
    let deltaValue = 0
    for (const totalIndex of totalIndices) {
      const d = currentRow[totalIndex] - previousRow[totalIndex]
      if (d < 0) {
        deltaValue = NaN
      }
      deltaValue += d
    }
    currentRow.push((previousRow[timestampIndex] + currentRow[timestampIndex]) / 2)
    currentRow.push(deltaValue / deltaTime * timeUnitSeconds)
    previousRow = currentRow
  }
}

function prepareData () {
  const secondsPerHour = 60 * 60

  for (const meter of meters) {
    switch (meter.type) {
      case 'electricity':
        addDeltaColumn(meter.readings, ['totalConsumptionKwhLow', 'totalConsumptionKwhHigh'], 'currentConsumptionW', 1000 * secondsPerHour)
        break
      case 'gas':
        addDeltaColumn(meter.readings, ['totalConsumptionM3'], 'currentConsumptionM3PerH', secondsPerHour)
        break
    }

    const timestampIndex = meter.readings[0].indexOf('timestamp')
    const centerTimestampIndex = meter.readings[0].indexOf('centerTimestamp')
    for (let i = 1; i < meter.readings.length; i++) {
      const row = meter.readings[i]
      row[timestampIndex] = new Date(row[timestampIndex] * 1000)
      row[centerTimestampIndex] = new Date(row[centerTimestampIndex] * 1000)
    }
  }
}

prepareData()
const vm = new Vue({
  el: '#main',
  data: { meters }
})
