const CleanWebpackPlugin = require('clean-webpack-plugin')
const path = require('path')

const rootPathTo = pathFromRoot => path.resolve(__dirname, pathFromRoot)

const distPath = rootPathTo('static')

const babelOptions = {
  presets: [
    [
      'env',
      {
        targets: {
          browsers: ['>1%', 'last 2 versions']
        },
        // https://github.com/babel/babel/tree/master/experimental/babel-preset-env#usebuiltins
        // Requires babel-core >= 7 (otherwise just true and false are supported).
        // TODO Once Babel 7 stabilizes, set to 'usage' and remove require('babel-polyfill') from main.js
        // useBuiltIns: 'usage'
        useBuiltIns: true
      }
    ]
  ]
}

module.exports = {
  entry: {
    main: './assets/js/main.js'
    // polyfill: './assets/js/polyfill.js'
  },
  output: {
    path: distPath,
    filename: '[name].js',
    publicPath: '/static/'
  },
  module: {
    rules: [
      {
        test: /\.js$/,
        include: rootPathTo('assets/js'),
        loader: 'babel-loader',
        options: babelOptions
      }
    ]
  },
  plugins: [
    // https://webpack.js.org/guides/output-management/#cleaning-up-the-dist-folder
    new CleanWebpackPlugin([distPath], {
      verbose: false
    })
  ],
  context: __dirname,
  target: 'web',
  // https://webpack.js.org/configuration/stats/
  stats: {
    chunks: false,
    colors: true,
    modules: false,
    performance: false,
    warnings: true
  }
}
