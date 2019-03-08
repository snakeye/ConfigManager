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
                value="123"
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
        }
    }
};
</script>

<style lang="scss">
.settings-group {
    @apply border p-4 mb-4 rounded;
}
</style>