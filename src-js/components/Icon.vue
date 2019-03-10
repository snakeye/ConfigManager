<template>
    <span class="icon-component inline-flex" v-html="require(`../icons/${icon}.svg`)" />
</template>

<script>
export default {
    props: {
        icon: {
            type: String,
            required: true,
        },
    },
};
</script>

<style lang="scss">
.icon-component {
    > svg {
        @apply w-full h-full;
        fill: currentColor;
    }
}
</style>
