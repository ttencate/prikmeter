require('babel-polyfill')

const Vue = require('../../node_modules/vue/dist/vue.common.js')

google.charts.load('current', {'packages': ['corechart']})
google.charts.setOnLoadCallback(init)

prepareData()

Vue.component('meters', {
  props: ['meters'],
  methods: {
    type: function (meter) {
      return meter.type
    }
  },
  template:
`<div>
  <div v-for="meter of meters" v-bind:key="meter.id">
    <h3>{{ type(meter) }}: <code>{{ meter.id }}</code></h3>
    <meter-readings v-bind:meter="meter"></meter-readings>
  </div>
</div>`
})

Vue.component('meter-readings', {
  props: ['meter'],
  computed: {
    data: function () {
      if (!this.meter.readings.dataTable) {
        const data = this.meter.readings.data
        this.meter.readings.data.unshift(this.meter.readings.keys)
        this.meter.readings.dataTable = google.visualization.arrayToDataTable(data)
      }

      const dataView = new google.visualization.DataView(this.meter.readings.dataTable)
      function getColumnIndex (label) {
        const n = dataView.getNumberOfColumns()
        for (let i = 0; i < n; i++) {
          if (dataView.getColumnLabel(i) === label) {
            return i
          }
        }
        return -1
      }

      switch (this.meter.type) {
        case 'electricity':
          dataView.setColumns(['centerTimestamp', 'currentConsumptionW'].map(getColumnIndex))
          break
        case 'gas':
          dataView.setColumns(['centerTimestamp', 'currentConsumptionM3PerH'].map(getColumnIndex))
          break
      }

      return dataView
    },
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
    }
  },
  template: '<column-chart v-bind:data="data" v-bind:options="options"></column-chart>'
})

// TODO use async component, https://vuejs.org/v2/guide/components.html#Async-Components
Vue.component('column-chart', {
  props: ['data', 'options'],
  template: '<div></div>',
  mounted: function () {
    const chart = new google.visualization.ColumnChart(this.$el)
    chart.draw(this.data, this.options)
  }
})

function init () {
  const vm = new Vue({
    el: '#main',
    data: { meters }
  })
}

function addDeltaColumn (readings, totalKeys, deltaKey, timeUnitSeconds) {
  const timestampIndex = readings.keys.indexOf('timestamp')
  const totalIndices = totalKeys.map(totalKey => readings.keys.indexOf(totalKey))
  readings.keys.push('centerTimestamp')
  readings.keys.push(deltaKey)
  let previousRow = readings.data[0]
  previousRow.push(NaN)
  previousRow.push(NaN)
  for (let i = 1; i < readings.data.length; i++) {
    const currentRow = readings.data[i]
    const deltaTime = currentRow[timestampIndex] - previousRow[timestampIndex]
    let deltaValue = 0
    for (let totalIndex of totalIndices) {
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

    const timestampIndex = meter.readings.keys.indexOf('timestamp')
    const centerTimestampIndex = meter.readings.keys.indexOf('centerTimestamp')
    for (const row of meter.readings.data) {
      row[timestampIndex] = new Date(row[timestampIndex] * 1000)
      row[centerTimestampIndex] = new Date(row[centerTimestampIndex] * 1000)
    }
  }
}
