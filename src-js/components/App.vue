<template>
    <div class="container m-auto">
        <v-wifi />

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
