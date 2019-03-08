<template>
    <div class="max-w-md mx-auto px-4 mb-8">
        <h1 class="mb-8">
            Configuration Manager
        </h1>

        <v-wifi class="mb-8" />

        <hr class="border-t my-4" />

        <v-settings />
    </div>
</template>

<script>
import WiFi from "./WiFi.vue";
import Settings from "./Settings.vue";

export default {
    components: {
        "v-wifi": WiFi,
        "v-settings": Settings
    }
};
</script>

<style lang="scss">
</style>
