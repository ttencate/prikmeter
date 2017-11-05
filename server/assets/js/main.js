google.charts.load('current', {'packages': ['corechart']})
google.charts.setOnLoadCallback(drawCharts)

prepareData()

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

    console.log(meter.readings.keys)
    console.log(meter.readings.data[0])
    console.log(meter.readings.data[1])
    console.log(meter.readings.data[2])
    console.log(meter.readings.data[3])
  }
}

function createDataTables () {
  for (const meter of meters) {
    const data = meter.readings.data
    meter.readings.data.unshift(meter.readings.keys)
    meter.readings.dataTable = google.visualization.arrayToDataTable(data)
  }
}

function drawCharts () {
  createDataTables()

  const chartDivs = document.getElementsByClassName('meterReadings')
  for (const div of chartDivs) {
    const index = div.dataset['index']
    drawChart(meters[index], div)
  }
}

function drawChart (meter, div) {
  const dataView = new google.visualization.DataView(meter.readings.dataTable)
  function getColumnIndex (label) {
    const n = dataView.getNumberOfColumns()
    for (let i = 0; i < n; i++) {
      if (dataView.getColumnLabel(i) === label) {
        return i
      }
    }
    return -1
  }

  let vAxisTitle
  let barColor
  switch (meter.type) {
    case 'electricity':
      dataView.setColumns(['centerTimestamp', 'currentConsumptionW'].map(getColumnIndex))
      barColor = '#FF851B'
      vAxisTitle = 'Electricity consumption (W)'
      break
    case 'gas':
      dataView.setColumns(['centerTimestamp', 'currentConsumptionM3PerH'].map(getColumnIndex))
      barColor = '#0074D9'
      vAxisTitle = 'Gas consumption (m³/h)'
      break
  }

  const options = {
    title: meter.id,
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
  const chart = new google.visualization.ColumnChart(div)
  chart.draw(dataView, options)
}
