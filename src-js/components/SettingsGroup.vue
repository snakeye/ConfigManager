<template>
    <div class="settings-group">
        <div class="mb-4">
            <h3>{{ group.label }}</h3>

            <p class="text-sm">
                {{ group.description }}
            </p>
        </div>

        <div
            v-for="param in group.params"
            :key="`settings-group-${group.name}-${param.name}`"
            class=""
        >
            <v-settings-param
                :group-name="group.name"
                :param="param"
                :value="getValue(param.name)"
                @param-updated="onParamUpdate"
            />
        </div>
    </div>
</template>

<script>
import SettingsParam from "./SettingsParam.vue";

export default {
    components: {
        "v-settings-param": SettingsParam
    },
    props: {
        group: {
            type: Object,
            required: true
        },
        values: {
            type: Object,
            required: true
        }
    },
    methods: {
        getValue(param) {
            return this.values[param];
        },
        onParamUpdate(param, value) {
            this.$emit('param-update', this.group.name, param, value);
        }
    }
};
</script>

<style lang="scss">
.settings-group {
    @apply border p-4 mb-4 rounded;
}
</style>