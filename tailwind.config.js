module.exports = {
    theme: {
        extend: {
            colors: {
                main: {
                    default: '#656d9a',
                    dark: '#36374c',
                    light: '#a4abc5;'
                },
                primary: {
                    default: '#7680ff',
                    light: 'rgba(118,128,255,.1)',
                },
                secondary: {
                    default: '#2ddab5',
                    light: 'rgba(45,218,181,.08)',
                }
            }
        }
    },
    variants: {},
    plugins: [],
    purge:{
        content:[
            './src-js/**/*.html',
            './src-js/**/*.vue',
            './src-js/**/*.jsx',
        ]
    }
}