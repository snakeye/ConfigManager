<template>
    <div v-if="param.type === 'bool'" class="form-group">
        <input :id="paramId" type="checkbox" v-model="inputValue">
        <label :for="paramId">{{ param.label }}</label>
    </div>
    <div v-else class="form-group">
        <label :for="paramId">{{ param.label }}</label>
        <input :id="paramId" type="text" v-model="inputValue" class="form-control mt-1">
        <p v-if="param.description" class="text-xs text-main-light">{{ param.description }}</p>
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
    data() {
        return {
            inputValue: this.value
        };
    },
    computed: {
        isTextInput() {
            return type === "int" || type === "string";
        },
        paramId() {
            return "input-" + this.groupName + "-" + this.param.name;
        }
    },
    watch: {
        value(newValue) {
            this.inputValue = newValue;
        },
        inputValue(newValue) {
            this.$emit("param-updated", this.param.name, newValue);
        }
    },
    beforeMount() {
        this.inputValue = this.value;
    }
};
</script>

<style lang="scss">
</style>
