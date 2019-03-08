<template>
    <div class="container m-auto">
        <h1>Configuration Manager</h1>

        <v-wifi class="mb-8"/>

        <v-settings />
    </div>
</template>

<script>
import WiFi from './WiFi.vue';
import Settings from './Settings.vue';

export default {
    components: {
        'v-wifi': WiFi,
        'v-settings': Settings,
    }
};
</script>

<style lang="scss">
</style>
