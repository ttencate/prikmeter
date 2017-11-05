require('babel-polyfill')
require('whatwg-fetch')

const Vue = require('../../node_modules/vue/dist/vue.common.js')

// https://developers.google.com/chart/interactive/docs/release_notes#official-releases
google.charts.load('45.2', {'packages': ['corechart']})

Vue.component('MeterReadings', {
  props: {
    meterId: String,
    meterType: String
  },
  data: function () {
    return {
      readings: null,
      error: null
    }
  },
  computed: {
    options: function () {
      const vAxisTitles = {
        electricity: 'Electricity consumption (W)',
        gas: 'Gas consumption (m³/h)'
      }
      const barColors = {
        electricity: '#FF851B',
        gas: '#0074D9'
      }
      const options = {
        title: this.meterId,
        width: 900,
        height: 300,
        bar: {
          groupWidth: '100%'
        },
        colors: [barColors[this.meterType]],
        hAxis: {
          title: 'Time',
          format: 'EEE\nd MMM'
        },
        vAxis: {
          title: vAxisTitles[this.meterType]
        },
        legend: {
          position: 'none'
        }
      }
      return options
    },
    yColumn: function () {
      return {
        electricity: 'currentConsumptionW',
        gas: 'currentConsumptionM3PerH'
      }[this.meterType]
    }
  },
  mounted: async function () {
    const response = await fetch(`/meters/${this.meterId}/readings`, { credentials: 'same-origin' })
    if (!response.ok) {
      this.$data.error = response.statusText
      return
    }

    const readings = await response.json()

    const secondsPerHour = 60 * 60
    switch (this.meterType) {
      case 'electricity':
        addDeltaColumn(readings, ['totalConsumptionKwhLow', 'totalConsumptionKwhHigh'], 'currentConsumptionW', 1000 * secondsPerHour)
        break
      case 'gas':
        addDeltaColumn(readings, ['totalConsumptionM3'], 'currentConsumptionM3PerH', secondsPerHour)
        break
    }

    const timestampIndex = readings[0].indexOf('timestamp')
    const centerTimestampIndex = readings[0].indexOf('centerTimestamp')
    for (let i = 1; i < readings.length; i++) {
      const row = readings[i]
      row[timestampIndex] = new Date(row[timestampIndex] * 1000)
      row[centerTimestampIndex] = new Date(row[centerTimestampIndex] * 1000)
    }

    this.$data.readings = readings
  },
  template:
`<ColumnChart v-if="readings" :data="readings" x-column="centerTimestamp" :y-column="yColumn" :options="options"/>
<p v-else-if="error">Error fetching meter readings: {{ error }}</p>
<p v-else>Loading…</p>`
})

Vue.component('ColumnChart', googleChartsComponent({
  props: {
    data: Array,
    xColumn: String,
    yColumn: String,
    options: Object
  },
  template: '<div/>',
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

function googleChartsComponent (component) {
  return function (resolve, reject) {
    // Adding a callback after it's already loaded will just immediately call
    // the callback. This is undocumented, but sensible enough that we can rely
    // on it.
    google.charts.setOnLoadCallback(function () {
      resolve(component)
    })
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

new Vue({
  el: '#main'
})
