<template>
    <div>
        <button @click="onReboot">Reboot</button>
    </div>
</template>

<script>
import { URL_REBOOT } from '../lib/api';
import { post } from "../lib/request";

export default {
    methods: {
        onReboot() {
            post(URL_REBOOT);
        }
    }

}
</script>