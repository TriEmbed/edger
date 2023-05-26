<template>
  <FormDrawer
    v-model="visible"
    :loading="loading"
    :width="680"
    flat
    title="Interface Information"
  >
    <template #content>
      <v-form ref="form">
        <v-container>
          <v-row>
            <v-col cols="3">
              <v-text-field
                v-model="formData.mDNS"
                label="mDNS index"
                readonly="true"
              />
            </v-col>
            <v-col cols="3">
              <v-text-field
                v-model="formData.IP"
                label="IP Address"
                readonly="true"
              />
            </v-col>
            <v-col cols="3">
              <v-text-field
                v-model="formData.MAC"
                label="MAC"
                readonly="true"
              />
            </v-col>
          </v-row>


          <v-row>
            <v-col cols="2">
              <v-text-field
                v-model="formData.CPUType"
                label="CPU"
                readonly="true"
              />
            </v-col>
            <v-col cols="2">
              <v-text-field
                v-model="formData.DeviceInstanceRevision"
                label="Revision"
                readonly="true"
              />
            </v-col>
            <v-col cols="3">
              <v-text-field
                v-model="formData.EMBFlash"
                label="EMB Flash"
                readonly="true"
              />
            </v-col>
            <v-col cols="3">
              <v-text-field
                v-model="formData.DEVICEINSTANCEFeatures"
                label="Features"
                readonly="true"
              />
            </v-col>
            <v-col cols="4">
              <v-text-field
                v-model="formData.MinimumFreeHeapSize"
                label="Minimum Free Heap Size"
                readonly="true"
              />
            </v-col>
            <v-col cols="4">
              <v-text-field
                v-model="formData.FreeHeapSize"
                label="Free Heap Size"
                readonly="true"
              />
            </v-col>
          </v-row>
          <v-row>
            <v-col cols="6">
              <v-text-field
                v-model="formData.Time"
                label="RTC current Time"
                readonly="true"
              />
            </v-col>
          </v-row>

          <v-row>
            <v-col cols="6">
              <v-text-field
                v-model="formData.ResetReason"
                label="Reset Reason"
                readonly="true"
              />
            </v-col>
          </v-row>
          <v-row>
            <v-col cols="3">
              <v-text-field
                v-model="formData.CompileDate"
                label="Compile Date"
                readonly="true"
              />
            </v-col>
            <v-col cols="2">
              <v-text-field
                v-model="formData.CompileTime"
                label="Compile Time"
                readonly="true"
              />
            </v-col>
            <v-col cols="6">
              <v-text-field
                v-model="formData.IDFVersion"
                label="IDF Version"
                readonly="true"
              />
            </v-col>
          </v-row>
          <v-row>
            <v-col cols="6">
              <v-text-field
                v-model.number="formData.Name"

                :rules="[
                  v => !!v || 'Please enter index',
                  v => v && v.length <= 32 || 'Enter up to 32 characters'
                ]"
                counter="32"
                label="index"
              />
            </v-col>
          </v-row>
          <v-row>
            <v-col
              class="py-0"
              cols="12"
            >
              <v-text-field
                v-model="formData.comment"
                :rules="[
                  v => !!v || 'Please enter Comments',
                  v => v && v.length <= 120 || 'Enter up to 120 characters'
                ]"
                counter="120"
                label="Comments"
              />
            </v-col>
          </v-row>
        </v-container>
      </v-form>
    </template>

    <template #actions>
      <v-spacer />
      <v-btn
        text
        x-large
        @click="close"
      >
        Cancel
      </v-btn>
      <v-btn
        text
        type="submit"
        x-large
        @click.stop.prevent="submit"
      >
        save
      </v-btn>
    </template>
  </FormDrawer>
</template>

<script>

import {addDevice, editDevice, getDevice} from '@/api/device'
import _ from 'lodash-es'

export default {
  header: [
    {
      text: 'Number',
      align: 'center',
      sortable: false,
      value: 'number',
      width: 50,
      fixed: true,
    },
    {
      text: 'mDNS',
      align: 'center',
      sortable: false,
      value: 'mDNS',
    },
    {
      text: 'ip Address',
      align: 'center',
      value: 'IP',
      width: 100,
    },
    {
      text: 'mac address',
      align: 'center',
      sortable: false,
      value: 'MAC',
      width: 120,
    },
    {
      text: 'CPU',
      align: 'center',
      value: 'CPUType',
      width: 120,
    },
    {
      text: 'Name',
      align: 'center',
      sortable: false,
      value: 'Name',
      width: 120,
    },
    {
      text: 'SPI Flash Chip MByte',
      align: 'center',
      value: 'SPIFlashChipMByte',
      width: 80,
    },
    {
      text: 'Compile Time',
      align: 'center',
      value: 'CompileTime',
      width: 100,
    },
    {
      text: 'Compile Date',
      align: 'center',
      value: 'CompileDate',
      width: 150,
    },
    {
      text: 'Operation',
      align: 'center',
      sortable: false,
      value: 'actions',
      width: 110,
      fixed: true,
    }],
  deviceinstance: [{
    "mDNS": "deviceinstance-interface.local",
    "Version": "v4.4.1-585-g145c3cd000-dirty",
    "Name": "friendly index",
    "Comments": "comments",
    "Cores": 1,
    "IP": "192.168.100.103",
    "DEVICEINSTANCEFeatures": 18,
    "DeviceInstanceRevision": 3,
    "SPIFlashChipMByte": 4,
    "EMBFlash": "External",
    "MAC": "58.CF.79.07.4A.90",
    "CPUType": "DEVICEINSTANCE32-C3",
    "ResetReason": "Reset due to power on event",
    "Time": "Thu Jan  1 00:00:14 1970",
    "CompileDate": "Feb  1 2023",
    "CompileTime": "11:24:50",
    "IDFVersion": "v4.4.1-585-g145c3cd000-dirty",
    "MinimumFreeHeapSize": 209044,
    "FreeHeapSize": 212384,
    "tag": "Interface",
  }],
  "tag": "Interface",
  name: 'InterfaceSchema',
  props: {},
  data: () => ({
    formData: {
      id: '',
      CPUType: '',
      IP: '',
      mDNS: '',
      DeviceInstanceRevision: '',
      DEVICEINSTANCEFeatures: '',
      MAC: '',
      Cores: '',
      SPIFlashChipMByte: "",
      EMBFlash: "",
      MinimumFreeHeapSize: "",
      Time: "",
      ResetReason: "",
      CompileDate: "",
      CompileTime: "",
      IDFVersion: "",
      FreeHeapSize: "",
      Name: "",
      Comments: "",
    },
    loading: false,
    visible: false,
  }),
  methods: {
    async getItem (id) {
      // this calls into the ap/device
      await getDevice(id).then((k) => {
        console.log(k.data)
      })
    },
    async add () {
      await addDevice(this.formData)
      this.$emit('addSuccess')
    },
    async edit () {
      await editDevice(this.formData)
      this.$emit('editSuccess')
    },
    async open (item) {
      try {
        this.visible = true
        this.loading = true
        if (item) {

          this.formData = _.pick(item, Object.keys(this.formData))
        }
      } finally {
        this.loading = false
      }
    },
    async close () {
      this.visible = false
      await this.$nextTick()
      Object.assign(this, this.$options.data.apply(this))
      this.$refs['form'].resetValidation()
      this.$refs['upload'].reset()
    },
    async submit () {
      if (!this.$refs['form'].validate()) return
      try {
        this.loading = true
        this.formData.id ? await this.edit() : await this.add()
        await this.close()
      } finally {
        this.loading = false
      }
    },
  },
}
</script>

<style lang="scss">
</style>
