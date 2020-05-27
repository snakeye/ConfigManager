<template>
    <div class="max-w-md mx-auto p-4 bg-white">
        <h1 class="page-heading">
            Configuration Manager
        </h1>

        <div class="mt-8">
            <div class="flex flex-row items-center border-b mb-4">
                <a href="#" @click.prevent="onSelectTab(1)" class="tab-selector" :class="{'--selected': selectedTab === 1}">
                    <v-icon icon="wifi-solid" class="w-5 h-5 mr-2"/>
                    WiFi
                </a>
                <a href="#" @click.prevent="onSelectTab(2)" class="tab-selector" :class="{'--selected': selectedTab === 2}">
                    <v-icon icon="cog-solid" class="w-5 h-5 mr-2"/>
                    Settings
                </a>
                <a href="#" @click.prevent="onSelectTab(3)" class="tab-selector" :class="{'--selected': selectedTab === 3}">
                    Manage
                </a>
            </div>
            <v-wifi v-if="selectedTab === 1"/>
            <v-settings v-if="selectedTab === 2" />
            <v-status v-if="selectedTab === 3" />
        </div>
    </div>
</template>

<script>
import Status from "./Status.vue";
import WiFi from "./WiFi.vue";
import Settings from "./Settings.vue";

export default {
    components: {
        "v-status": Status,
        "v-wifi": WiFi,
        "v-settings": Settings
    },
    data: () => ({
        selectedTab: 1
    }),
    methods: {
        onSelectTab(tab) {
            this.selectedTab = tab;
        }
    }
};
</script>

<style lang="scss">
.tab-selector {
    @apply inline-flex items-center py-1 px-2 bg-white text-sm;

    &.--selected {
        @apply bg-gray-100 text-primary;
    }
}
</style>
