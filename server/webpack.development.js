const webpack = require('webpack')
const webpackMerge = require('webpack-merge')

const webpackCommon = require('./webpack.common')

// https://webpack.js.org/guides/production/#setup
module.exports = webpackMerge(webpackCommon, {
  mode: 'development',
  // https://webpack.js.org/guides/development/#using-source-maps
  devtool: 'inline-source-map',
  plugins: [
    // https://webpack.js.org/guides/production/#specify-the-environment
    new webpack.DefinePlugin({
      'process.env': {
        NODE_ENV: JSON.stringify('development')
      }
    }),
  ]
})
