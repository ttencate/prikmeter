require('babel-polyfill')
require('whatwg-fetch')

const roundDate = require('round-date')
const Vue = require('../../node_modules/vue/dist/vue.common.js')

// https://developers.google.com/chart/interactive/docs/release_notes#official-releases
google.charts.load('45.2', {'packages': ['corechart']})

function defaultChartOptions (meterType, vAxisMin, vAxisMax) {
  let consumptionColor
  let vAxisTitle
  // http://paletton.com/#uid=70c0X0krKw0hhHhmv-HvKrezxln
  switch (meterType) {
    case 'electricity':
      consumptionColor = '#FF5722'
      vAxisTitle = 'Elektriciteit (W)'
      break
    case 'gas':
      consumptionColor = '#2D36B1'
      vAxisTitle = 'Gas (m³/h)'
      break
  }
  const vAxis = {
    title: vAxisTitle,
    titleTextStyle: {
      italic: false
    },
    viewWindow: {
      min: vAxisMin,
      max: vAxisMax
    }
  }
  return {
    width: '100%',
    height: 200,
    title: '',
    series: {
      // Consumption
      0: {
        type: 'bars',
        color: consumptionColor,
        targetAxisIndex: 0,
      },
      // Production
      1: {
        type: 'bars',
        color: '#18B364',
        targetAxisIndex: 1,
      },
      // Net consumption
      2: {
        type: 'line',
        color: '#FFC422',
        targetAxisIndex: 0,
      },
    },
    bar: {
      groupWidth: '90%'
    },
    isStacked: true,
    hAxis: {},
    vAxes: {
      0: vAxis,
      1: vAxis
    },
    chartArea: {
      left: '10%',
      width: '90%',
      top: '12.5%',
      height: '75%',
    },
    legend: {
      position: 'none'
    },
    focusTarget: 'category',
    aggregationTarget: 'category',
    tooltip: {
      isHtml: true
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
    let consumptionColumn
    let productionColumn
    let netConsumptionColumn
    switch (this.$props.meterType) {
      case 'electricity':
        consumptionColumn = 'currentConsumptionW'
        productionColumn = 'currentProductionW'
        netConsumptionColumn = 'currentNetConsumptionW'
        break
      case 'gas':
        consumptionColumn = 'currentConsumptionM3PerH'
        break
    }
    return {
      now: now,
      days: days,
      consumptionColumn: consumptionColumn,
      productionColumn: productionColumn,
      netConsumptionColumn: netConsumptionColumn,
      readingsByHour: null,
      readingsByMinute: null,
      vAxisMin: 0,
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
          addDeltaColumn(readings, ['totalProductionKwhLow', 'totalProductionKwhHigh'], 'currentProductionW', intervalMillis, timeUnitMillis * 1000 /* kW -> W */)
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

    const maxValue = (readings, columnName) => {
      let max = 0
      let columnIndex = readings[0].indexOf(columnName)
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
    this.$data.vAxisMin = -roundUpToNiceNumber(Math.max(
        maxValue(readingsByMinute, this.$data.productionColumn),
        maxValue(readingsByHour, this.$data.productionColumn)))
    this.$data.vAxisMax = roundUpToNiceNumber(Math.max(
        maxValue(readingsByMinute, this.$data.consumptionColumn),
        maxValue(readingsByHour, this.$data.consumptionColumn)))
  },
  template: `
    <div v-if="readingsByHour">
      <div v-if="meterType == 'electricity'">
        <MeterReadingsChart
            :meterType="meterType"
            :data="readingsByMinute"
            :title="'Afgelopen uur'"
            :consumptionColumn="consumptionColumn"
            :productionColumn="productionColumn"
            :netConsumptionColumn="netConsumptionColumn"
            :vAxisMin="vAxisMin"
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
            :consumptionColumn="consumptionColumn"
            :productionColumn="productionColumn"
            :netConsumptionColumn="netConsumptionColumn"
            :vAxisMin="vAxisMin"
            :vAxisMax="vAxisMax"
            :endTime="new Date(day.getFullYear(), day.getMonth(), day.getDate() + 1)"
            :durationMillis="24 * 60 * 60 * 1000"
            :majorGridlines="8"
            :minorGridlines="3"/>
      </div>
    </div>
    <p v-else-if="error">Fout bij ophalen van meterstanden: {{ error }}</p>
    <p v-else>Bezig met laden…</p>
  `
})

Vue.component('MeterReadingsChart', {
  props: {
    meterType: String,
    data: Array,
    title: String,
    consumptionColumn: String,
    productionColumn: String,
    netConsumptionColumn: String,
    vAxisMin: Number,
    vAxisMax: Number,
    endTime: Date,
    durationMillis: Number,
    majorGridlines: Number,
    minorGridlines: Number
  },
  computed: {
    options: function () {
      const options = defaultChartOptions(this.$props.meterType, this.$props.vAxisMin, this.$props.vAxisMax)
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
    <ComboChart
      :data="data"
      :xColumn="'centerTimestamp'"
      :consumptionColumn="consumptionColumn"
      :productionColumn="productionColumn"
      :netConsumptionColumn="netConsumptionColumn"
      :options="options"
    />
  `
})

Vue.component('ComboChart', googleChartsComponent({
  props: {
    data: Array,
    xColumn: String,
    consumptionColumn: String,
    productionColumn: String,
    netConsumptionColumn: String,
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
    const xColumnIndex = getColumnIndex(this.xColumn)
    const consumptionColumnIndex = getColumnIndex(this.consumptionColumn)
    const productionColumnIndex = this.productionColumn ? getColumnIndex(this.productionColumn) : null

    const columns = []

    columns.push(xColumnIndex)

    // Add tooltip. By putting this right after the xColumnIndex, it applies to
    // the entire "category" = row.
    columns.push({
      calc: (dataTable, rowIndex) => {
        let parts = []
        parts.push(`<strong>${dataTable.getValue(rowIndex, xColumnIndex).toLocaleString('nl-NL')}</strong><br>`)
        parts.push('<dl>')
        const consumption = dataTable.getValue(rowIndex, consumptionColumnIndex)
        parts.push('<dt>Verbruik:</dt>')
        parts.push(`<dd>${dataTable.getValue(rowIndex, consumptionColumnIndex)}</dd>`)
        if (this.productionColumn) {
          const production = dataTable.getValue(rowIndex, productionColumnIndex)
          parts.push('<dt>Opwek:</dt>')
          parts.push(`<dd>${typeof production === 'number' ? production : 'onbekend'}</dd>`)
          const netConsumption = typeof consumption === 'number' && typeof production === 'number' ? consumption - production : consumption;
          parts.push('<dt>Netto verbruik (verbruik − opwek):</dt>')
          parts.push(`<dd>${typeof netConsumption === 'number' ? netConsumption : 'onbekend'}</dd>`)
        }
        parts.push('</dl>')
        return parts.join('')
      },
      type: 'string',
      role: 'tooltip',
      properties: { html: true }
    })

    columns.push(consumptionColumnIndex)
    if (this.productionColumn) {
      columns.push({
        calc: (dataTable, rowIndex) => {
          const production = dataTable.getValue(rowIndex, productionColumnIndex)
          return typeof production === 'number' ? -production : production
        },
        type: 'number',
        label: this.productionColumn + 'Negated',
      })

      columns.push({
        calc: (dataTable, rowIndex) => {
          const consumption = dataTable.getValue(rowIndex, consumptionColumnIndex)
          const production = dataTable.getValue(rowIndex, productionColumnIndex)
          return typeof consumption === 'number' && typeof production === 'number' ? consumption - production : consumption;
        },
        type: 'number',
        label: this.productionColumn + 'Net',
      })
    }

    dataView.setColumns(columns)

    const chart = new google.visualization.ComboChart(this.$el)
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
