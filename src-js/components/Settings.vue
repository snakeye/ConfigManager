<template>
    <div>
        <h2>App settings</h2>

        <div v-for="group in groups" :key="`settings-group-${group.name}`">
            <v-settings-group :group="group" />
        </div>

        <div class="text-right">
            <button class="btn --secondary mr-4">Reset</button>
            <button class="btn --primary ">Save</button>
        </div>
    </div>
</template>

<script>
import SettingsGroup from "./SettingsGroup.vue";

export default {
    components: {
        "v-settings-group": SettingsGroup
    },
    data: () => ({
        groups: []
    }),
    beforeMount() {
        this.groups = [
            {
                name: "app",
                label: "Application",
                description: "Application settings",
                params: [
                    {
                        name: "name",
                        label: "Name",
                        type: "string"
                    },
                    {
                        name: "enabled",
                        label: "Enabled",
                        type: "bool"
                    },
                    {
                        name: "hour",
                        label: "Hour",
                        type: "int"
                    }
                ]
            }
        ];
    }
};
</script>

<style lang="scss">
</style>