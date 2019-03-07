<template>
    <div class="">
        {{ group.label }}

        <p>
            {{ group.description }}
        </p>

        <div
            v-for="param in group.params"
            :key="`settings-group-${group.name}-${param.name}`"
            class=""
        >
            <v-settings-param param="param" value="123"/>
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
</style>