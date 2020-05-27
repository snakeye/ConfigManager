<template>
    <div class="max-w-md mx-auto px-4 mb-8">
        <h1 class="mb-8">
            Configuration Manager
        </h1>

        <div class="mb-8 text-right">
            <button @click="onReboot">Reboot</button>
        </div>

        <v-wifi class="mb-8" />

        <hr class="border-t my-4" />

        <v-settings />
    </div>
</template>

<script>
import { post } from "../lib/request";

import WiFi from "./WiFi.vue";
import Settings from "./Settings.vue";
import { URL_REBOOT } from '../lib/api';

export default {
    components: {
        "v-wifi": WiFi,
        "v-settings": Settings
    },
    methods: {
        onReboot() {
            post(URL_REBOOT);
        }
    }
};
</script>

<style lang="scss">
</style>
