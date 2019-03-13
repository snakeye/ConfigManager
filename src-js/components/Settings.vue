<template>
    <div>
        <h2 class="mb-4">
            <v-icon icon="cog-solid" class="w-5 h-5 mr-2"/>App settings
        </h2>

        <div v-for="group in schema" :key="`settings-group-${group.name}`">
            <v-settings-group
                :group="group"
                :values="getValues(group.name)"
                @param-update="onParamUpdate"
            />
        </div>

        <div class="text-right">
            <button class="btn --secondary mr-4" @click="onReset">Reset</button>
            <button class="btn --primary" @click="onSave">Save</button>
        </div>
    </div>
</template>

<script>
import cloneDeep from "lodash/cloneDeep";

import SettingsGroup from "./SettingsGroup.vue";

import { get, options, post } from "../lib/request";
import { URL_SETTINGS } from "../lib/api";

export default {
    components: {
        "v-settings-group": SettingsGroup
    },
    data: () => ({
        isLoading: false,
        schema: [],
        initialValues: {},
        values: {}
    }),
    beforeMount() {
        this.isLoading = true;
        Promise.all([options(URL_SETTINGS), get(URL_SETTINGS)])
            .then(([schema, data]) => {
                this.schema = schema;
                this.initialValues = data;
                this.values = cloneDeep(data);
                this.isLoading = false;
            })
            .catch(err => {
                console.log(err);
                this.isLoading = false;
            });
    },
    methods: {
        getValues(group) {
            return this.values[group];
        },
        onParamUpdate(group, param, value) {
            this.values[group][param] = value;
        },
        onReset() {
            this.values = cloneDeep(this.initialValues);
            this.$forceUpdate();
        },
        onSave() {
            this.isLoading = true;
            post(URL_SETTINGS, { body: this.values })
                .then(() => {
                    this.initialValues = cloneDeep(this.values);
                    this.isLoading = false;
                })
                .catch((err) => {
                    console.log(err);
                    this.isLoading = false;
                });
        }
    }
};
</script>

<style lang="scss">
</style>