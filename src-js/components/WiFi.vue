<template>
    <div class="">
        <h2>WiFi settings</h2>

        <p></p>

        <div class="settings-group">
            <div class="form-group">
                <label>Access point</label>
                <input type="text" class="form-control" />
            </div>

            <div class="form-group">
                <label>Password</label>
                <input type="password" class="form-control" />
            </div>
        </div>

        <div class="text-right">
            <button v-if="!connected" class="btn --primary">Connect</button>
            <button v-if="connected" class="btn --primary">Disconnect</button>
        </div>
    </div>
</template>

<script>
export default {};
</script>

<style lang="scss">
</style>