<template>
    <div>
        <h2 class="mb-4">
            <v-icon icon="cog-solid" class="w-5 h-5 mr-2"/>App settings
        </h2>

        <div v-for="group in schema" :key="`settings-group-${group.name}`">
            <v-settings-group :group="group" :values="getValues(group.name)"/>
        </div>

        <div class="text-right">
            <button class="btn --secondary mr-4">Reset</button>
            <button class="btn --primary">Save</button>
        </div>
    </div>
</template>

<script>
import SettingsGroup from "./SettingsGroup.vue";

import { get, options } from "../lib/request";
import { URL_SETTINGS } from "../lib/api";

export default {
    components: {
        "v-settings-group": SettingsGroup
    },
    data: () => ({
        isLoading: false,
        schema: [],
        values: {},
        newValues: {}
    }),
    beforeMount() {
        this.isLoading = true;
        Promise.all([options(URL_SETTINGS), get(URL_SETTINGS)])
            .then(([schema, data]) => {
                this.schema = schema;
                this.values = data;
                this.isLoading = false;
            })
            .catch(() => {
                this.isLoading = false;
            });
    },
    methods: {
        getValues(group) {
            return this.values[group];
        },
        save() {
            this.isLoading = true;

            post(URL_SETTINGS)
                .then(() => {
                    this.isLoading = false;
                })
                .catch(() => {
                    this.isLoading = false;
                });
        }
    }
};
</script>

<style lang="scss">
</style>