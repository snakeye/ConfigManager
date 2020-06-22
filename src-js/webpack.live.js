const path = require('path');
const glob = require('glob-all');
const webpack = require('webpack');
const merge = require('webpack-merge');

const PurgecssPlugin = require('purgecss-webpack-plugin');

const baseWebpackConfig = require('./webpack.base');

// Custom PurgeCSS extractor for Tailwind that allows special characters in
// class names.
//
// https://github.com/FullHuman/purgecss#extractor
class TailwindExtractor {
    static extract(content) {
        return content.match(/[A-Za-z0-9-_:/]+/g) || [];
    }
}

module.exports = merge(baseWebpackConfig, {
    mode: 'production',
    devtool: false,
    plugins: [
        new webpack.DefinePlugin({
            NODE_ENV: JSON.stringify('production'),
        }),
        new webpack.HashedModuleIdsPlugin(),
        // new PurgecssPlugin({
        //     paths: glob.sync([
        //         path.join('./src-js/index.html'),
        //         path.join('./src-js/**/*.vue'),
        //         path.join('./src-js/**/*.js'),
        //     ]),
        //     extractors: [{
        //         extractor: TailwindExtractor,
        //         // Specify the file extensions to include when scanning for
        //         // class names.
        //         extensions: ['html', 'js', 'vue'],
        //     }],
        // }),
    ],
});