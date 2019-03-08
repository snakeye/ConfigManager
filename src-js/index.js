import './css/main.scss';

import Vue from 'vue';

import App from './components/App.vue';

import Icon from './components/Icon.vue';

Vue.component('v-icon', Icon);

new Vue({
        el: '#app',
        render: h => h(App),
    });
