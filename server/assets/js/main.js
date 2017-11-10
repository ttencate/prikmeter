require('babel-polyfill')
require('whatwg-fetch')

const Vue = require('../../node_modules/vue/dist/vue.common.js')

// https://developers.google.com/chart/interactive/docs/release_notes#official-releases
google.charts.load('45.2', {'packages': ['corechart']})

function defaultChartOptions (meterType, vAxisMax) {
  const barColors = {
    electricity: '#FF851B',
    gas: '#0074D9'
  }
  const vAxisTitles = {
    electricity: 'Electricity consumption (W)',
    gas: 'Gas consumption (m³/h)'
  }
  return {
    width: 500,
    height: 200,
    title: '',
    bar: {
      groupWidth: '100%'
    },
    colors: [barColors[meterType]],
    hAxis: {},
    vAxis: {
      title: vAxisTitles[meterType],
      viewWindow: {
        min: 0,
        max: vAxisMax
      }
    },
    chartArea: {
      left: 50,
      width: 450,
      top: 25,
      height: 150,
    },
    legend: {
      position: 'none'
    }
  }
}

function roundUpToNiceNumber (value) {
  if (value <= 0) {
    return 0
  }
  const multipleOf10 = Math.pow(10, Math.floor(Math.log10(value)))
  const mantissa = value / multipleOf10
  const niceNumbers = [1, 1.2, 1.5, 2, 2.5, 3, 4, 5, 6, 8, 10]
  const roundedMantissa = niceNumbers.find(n => n >= mantissa)
  if (!roundedMantissa) {
    return value // This should not happen.
  }
  return roundedMantissa * multipleOf10
}

Vue.component('MeterReadings', {
  props: {
    meterId: String,
    meterType: String
  },
  data: function () {
    const yColumn = {
      electricity: 'currentConsumptionW',
      gas: 'currentConsumptionM3PerH'
    }[this.meterType]
    const now = new Date()
    const days = []
    for (let i = 0; i < 7; i++) {
      days.push(new Date(now.getFullYear(), now.getMonth(), now.getDate() - i))
    }
    return {
      yColumn: yColumn,
      days: days,
      readingsByHour: null,
      readingsByMinute: null,
      vAxisMax: 0,
      error: null,
    }
  },
  methods: {
    optionsForDay: function (day) {
      const options = defaultChartOptions(this.meterType, this.$data.vAxisMax)
      options.title = day.toDateString()
      const nextDay = new Date(day.getFullYear(), day.getMonth(), day.getDate() + 1)
      options.hAxis = {
        format: 'HH:mm', // For days, use: 'EEE\nd MMM'
        viewWindow: {
          min: day,
          max: nextDay
        },
        gridlines: {
          count: 8
        },
        minorGridlines: {
          count: 2
        }
      }
      return options
    },
    optionsForHour: function () {
      const options = defaultChartOptions(this.meterType, this.$data.vAxisMax)
      options.title = 'Past hour'
      options.hAxis = {
        format: 'HH:mm',
        gridlines: {
          count: 6,
        },
        minorGridlines: {
          count: 9
        }
      }
      return options
    }
  },
  mounted: async function () {
    const fetchReadings = async (query) => {
      const encodedQuery = Object.entries(query)
        .map(([ key, value ]) => `${encodeURIComponent(key)}=${encodeURIComponent(value)}`)
        .join('&')
      const response = await fetch(`/meters/${this.meterId}/readings?${encodedQuery}`, { credentials: 'same-origin' })
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

      return readings
    }

    const nowMillis = Date.now()
    const oneHourAgoMillis = new Date(nowMillis - 60 * 60 * 1000).getTime();
    const oneWeekAgoMillis = new Date(nowMillis - 7 * 24 * 60 * 60 * 1000).getTime();

    const [ readingsByMinute, readingsByHour ] = await Promise.all([
      await fetchReadings({ startTime: oneHourAgoMillis, endTime: nowMillis, resolution: 'minute' }),
      await fetchReadings({ startTime: oneWeekAgoMillis, endTime: nowMillis, resolution: 'hour' })
    ])
    const maxValue = (readings) => {
      let max = 0
      let columnIndex = readings[0].indexOf(this.$data.yColumn)
      for (let i = 1; i < readings.length; i++) {
        const reading = readings[i][columnIndex]
        if (!isNaN(reading)) {
          max = Math.max(max, reading)
        }
      }
      return max
    }

    this.$data.readingsByHour = readingsByHour;
    this.$data.readingsByMinute = readingsByMinute;
    this.$data.vAxisMax = roundUpToNiceNumber(Math.max(maxValue(readingsByMinute), maxValue(readingsByHour)))

    // TODO this is needed because optionsForDay(day) changed, but is horrendously bad style -- refactor to fix
    this.$forceUpdate()
  },
  template:
`<div v-if="readingsByHour">
  <div v-if="meterType == 'electricity'">
    <ColumnChart :data="readingsByMinute" x-column="centerTimestamp" :y-column="yColumn" :options="optionsForHour()"/>
  </div>
  <div v-for="day in days">
    <ColumnChart :data="readingsByHour" x-column="centerTimestamp" :y-column="yColumn" :options="optionsForDay(day)"/>
  </div>
</div>
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
  // TODO ensure that the server always sends headings, even if the data is empty, then remove this
  if (readings.length <= 1) {
    return
  }
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
