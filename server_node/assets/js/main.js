require('babel-polyfill')
require('whatwg-fetch')

const roundDate = require('round-date')
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
      groupWidth: '90%'
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
  const niceNumbers = [1, 1.2, 1.4, 1.6, 1.8, 2, 2.5, 3, 4, 5, 6, 8, 10]
  const roundedMantissa = niceNumbers.find(n => n >= mantissa)
  if (!roundedMantissa) {
    return value // This should not happen.
  }
  return roundedMantissa * multipleOf10
}

function parseHash (hash) {
  const obj = {}
  if (hash) {
    const parts = hash.substring(1, hash.length).split('&')
    for (const part of parts) {
      const [key, value] = part.split('=')
      obj[decodeURIComponent(key)] = decodeURIComponent(value)
    }
  }
  return obj
}

function getNow () {
  const hash = parseHash(window.location.hash)
  const hashNow = hash['now']
  if (hashNow) {
    const typedHashNow = /^\d+$/.test(hash['now']) ? parseInt(hashNow) : hashNow
    const now = new Date(typedHashNow)
    console.log(`Pretending current time is ${now}`)
    return now
  } else {
    return new Date()
  }
}

Vue.component('MeterReadings', {
  props: {
    meterId: String,
    meterType: String
  },
  data: function () {
    const now = getNow()
    const days = []
    for (let i = 0; i < 7; i++) {
      days.push(new Date(now.getFullYear(), now.getMonth(), now.getDate() - i))
    }
    const yColumn = {
      electricity: 'currentConsumptionW',
      gas: 'currentConsumptionM3PerH'
    }[this.$props.meterType]
    return {
      now: now,
      days: days,
      yColumn: yColumn,
      readingsByHour: null,
      readingsByMinute: null,
      vAxisMax: 0,
      error: null,
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

      // Convert date/time strings into real Date objects
      const timestampIndex = readings[0].indexOf('timestamp')
      for (let i = 1; i < readings.length; i++) {
        const row = readings[i]
        row[timestampIndex] = new Date(row[timestampIndex])
      }

      const INTERVALS = {
        second: 1000,
        minute: 60 * 1000,
        hour: 60 * 60 * 1000,
        day: 24 * 60 * 60 * 1000
      }
      const intervalMillis = INTERVALS[query.resolution]
      const timeUnitMillis = INTERVALS['hour']
      switch (this.meterType) {
        case 'electricity':
          addDeltaColumn(readings, ['totalConsumptionKwhLow', 'totalConsumptionKwhHigh'], 'currentConsumptionW', intervalMillis, timeUnitMillis * 1000 /* kW -> W */)
          break
        case 'gas':
          addDeltaColumn(readings, ['totalConsumptionM3'], 'currentConsumptionM3PerH', intervalMillis, timeUnitMillis)
          break
      }

      return readings
    }

    const nowMillis = this.$data.now.getTime()
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
  },
  template: `
    <div v-if="readingsByHour">
      <div v-if="meterType == 'electricity'">
        <MeterReadingsChart
            :meterType="meterType"
            :data="readingsByMinute"
            :title="'Past hour'"
            :yColumn="yColumn"
            :vAxisMax="vAxisMax"
            :endTime="now"
            :durationMillis="60 * 60 * 1000"
            :majorGridlines="6"
            :minorGridlines="10"/>
      </div>
      <div v-for="day in days">
        <MeterReadingsChart
            :meterType="meterType"
            :data="readingsByHour"
            :title="day.toDateString()"
            :yColumn="yColumn"
            :vAxisMax="vAxisMax"
            :endTime="new Date(day.getFullYear(), day.getMonth(), day.getDate() + 1)"
            :durationMillis="24 * 60 * 60 * 1000"
            :majorGridlines="8"
            :minorGridlines="3"/>
      </div>
    </div>
    <p v-else-if="error">Error fetching meter readings: {{ error }}</p>
    <p v-else>Loading…</p>
  `
})

Vue.component('MeterReadingsChart', {
  props: {
    meterType: String,
    data: Array,
    title: String,
    yColumn: String,
    vAxisMax: Number,
    endTime: Date,
    durationMillis: Number,
    majorGridlines: Number,
    minorGridlines: Number
  },
  computed: {
    options: function () {
      const options = defaultChartOptions(this.$props.meterType, this.$props.vAxisMax)
      options.title = this.$props.title
      options.hAxis = {
        format: 'HH:mm', // For days, use: 'EEE\nd MMM'
        viewWindow: {
          min: new Date(this.$props.endTime.getTime() - this.$props.durationMillis),
          max: this.$props.endTime
        },
        gridlines: {
          count: this.$props.majorGridlines // It's subdivisions, not lines.
        },
        minorGridlines: {
          count: this.$props.minorGridlines - 1 // It's lines, not subdivisions.
        }
      }
      return options
    }
  },
  template: `
    <ColumnChart
      :data="data"
      :x-column="'centerTimestamp'"
      :y-column="yColumn"
      :options="options"
    />
  `
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
    // TODO if there are no data rows, the column type is misdetected
    // and we get an error when drawing the chart:
    // Data column(s) for axis #0 cannot be of type string
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

function addDeltaColumn (readings, totalKeys, deltaKey, intervalMillis, timeUnitMillis) {
  const timestampIndex = readings[0].indexOf('timestamp')
  const totalIndices = totalKeys.map(totalKey => readings[0].indexOf(totalKey))

  readings[0].push('centerTimestamp', deltaKey)

  let previousRow = readings[1]
  if (!previousRow) {
    return
  }
  previousRow.push(new Date(NaN), NaN)

  for (let i = 2; i < readings.length; i++) {
    const currentRow = readings[i]
    let deltaValue = 0
    for (const totalIndex of totalIndices) {
      const d = currentRow[totalIndex] - previousRow[totalIndex]
      if (d < 0) {
        deltaValue = NaN
      }
      deltaValue += d
    }

    const centerTimestampMillis = (previousRow[timestampIndex].getTime() + currentRow[timestampIndex].getTime()) / 2
    const centerTimestampMillisRounded = roundDate.floor(intervalMillis / 1000, new Date(centerTimestampMillis - intervalMillis / 2)).getTime() + intervalMillis / 2
    const centerTimestamp = new Date(centerTimestampMillisRounded)

    const deltaTimeMillis = currentRow[timestampIndex].getTime() - previousRow[timestampIndex].getTime()
    const delta = deltaValue / deltaTimeMillis * timeUnitMillis

    currentRow.push(centerTimestamp, delta)

    previousRow = currentRow
  }
}

new Vue({ el: '#main' })
