<template>
    <div v-if="param.type === 'bool'" class="form-group">
        <input :id="paramId" type="checkbox" />
        <label :for="paramId">{{ param.label }}</label>
    </div>
    <div v-else class="form-group">
        <label :for="paramId">{{ param.label }}</label>
        <input :id="paramId" type="text" v-model="value" class="form-control" />
        <p v-if="param.description" class="text-xs">{{ param.description }}</p>
    </div>
</template>

<script>
export default {
    props: {
        groupName: {
            type: String,
            required: true
        },
        param: {
            type: Object,
            required: true
        },
        value: {
            type: Number | String | Boolean,
            required: true
        }
    },
    computed: {
        isTextInput() {
            return type === "int" || type === "string";
        },
        paramId() {
            return "input-" + this.groupName + "-" + this.param.name;
        }
    }
};
</script>

<style lang="scss">
</style>
    