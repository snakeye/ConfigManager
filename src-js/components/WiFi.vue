<template>
    <div class>
        <h2 class="mb-4">
            <v-icon icon="wifi-solid" class="w-5 h-5 mr-2"/>WiFi settings
        </h2>

        <div class="settings-group">
            <div class="form-group">
                <label>Access point</label>
                <input type="text" class="form-control" v-model="ssid">
            </div>

            <div class="form-group">
                <label>Password</label>
                <input type="password" class="form-control" v-model="password">
            </div>

            <div class="text-right mb-4">
                <button v-if="!connected" class="btn --primary" @click="onConnect" :disabled="!isConnectAvailable">Connect</button>
                <button v-if="connected" class="btn --primary" @click="onDisconnect">Disconnect</button>
            </div>

            <div class="border-t pt-4">
                <h3 class="mb-4">Available networks</h3>

                <div class="mb-4">
                    <button class="btn --secondary --sm" @click="scan">Scan</button>
                </div>

                <div
                    v-for="(net, index) in networks"
                    :key="`net-${index}`"
                    class="flex flex-row -mx-2 p-2 hover:bg-grey-lighter cursor-pointer"
                    @click="selectNetwork(net)"
                >
                    <div class="px-2">
                        <v-icon v-if="!net.open" icon="lock-solid" class="w-4 h-4"/>
                        <span v-else class="w-4 inline-flex"></span>
                    </div>

                    <div class="flex-1 px-2">{{ net.ssid }}</div>

                    <div class="px-2">{{ net.rssi }}</div>
                </div>
            </div>
        </div>
    </div>
</template>

<script>
import { get, post } from "../lib/request";
import {
    URL_WIFI_SCAN,
    URL_WIFI_CONNECT,
    URL_WIFI_DISCONNECT
} from "../lib/api";

export default {
    data: () => ({
        ssid: "",
        password: "",
        connected: false,
        isScanning: false,
        networks: []
    }),
    computed: {
        isConnectAvailable() {
            return this.ssid !== '';
        }
    },
    beforeMount() {
        this.scan();
    },
    methods: {
        selectNetwork(net) {
            this.ssid = net.ssid;
        },
        rssi(net) {
            if (net.rssi > -67) {
            } else if (net.rssi > -70) {
            } else if (net.rssi > -80) {
            } else {
            }
        },
        scan() {
            this.isScanning = true;
            get(URL_WIFI_SCAN)
                .then(networks => {
                    this.networks = networks;
                    this.isScanning = false;
                })
                .catch(() => {
                    this.isScanning = false;
                });
        },
        onConnect() {
            post(URL_WIFI_CONNECT, {
                body: { ssid: this.ssid, password: this.password }
            })
                .then(() => {})
                .catch(() => {});
        },
        onDisconnect() {
            post(URL_WIFI_DISCONNECT)
                .then(() => {})
                .catch(() => {});
        }
    }
};
</script>

<style lang="scss">
</style>