<template>
    <div v-if="schema.length > 0">
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
                this.values = JSON.parse(JSON.stringify(data));
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
            this.values = JSON.parse(JSON.stringify(this.initialValues));
            this.$forceUpdate();
        },
        onSave() {
            this.isLoading = true;
            post(URL_SETTINGS, { body: this.values })
                .then(() => {
                    this.initialValues = JSON.parse(JSON.stringify(values));
                    this.isLoading = false;
                })
                .catch(err => {
                    console.log(err);
                    this.isLoading = false;
                });
        }
    }
};
</script>

<style lang="scss">
</style>