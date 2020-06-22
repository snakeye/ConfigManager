<template>
    <div>
        <button @click="onReboot" class="btn --secondary">Reboot</button>
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