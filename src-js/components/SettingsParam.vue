<template>
    <div class="">
        <input type="text" v-model="value" class="border"/>
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
            type: Number || String || Boolean,
            required: true,
        }
    }
};
</script>

<style lang="scss">
</style>
    