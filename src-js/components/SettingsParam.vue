<template>
    <div v-if="param.type === 'bool'">
        <input type="checkbox"/>
        <label>{{ param.label }}</label>
    </div>
    <div v-else class="form-group">
        <label>{{ param.label }}</label>
        <input type="text" v-model="value" class="form-control"/>
    </div>
</template>

<script>
export default {
    props: {
        param: {
            type: Object,
            required: true
        },
        value: {
            type: Number | String | Boolean,
            required: true,
        }
    },
    computed: {
        isTextInput() {
            return type === 'int' || type === 'string'; 
        }
    }
};
</script>

<style lang="scss">
</style>
    