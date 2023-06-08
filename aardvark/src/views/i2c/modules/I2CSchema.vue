<template>
  <FormDrawer
    v-model="visible"
    :loading="loading"
    :width="680"
    flat
    title="step Information"
  >
    <template #content>
      <v-form ref="form">
        <v-container>
          <v-row>
            <v-col cols="3">
              <v-select
                label="Function"
                :items="['Read', 'Write', 'Wait', 'Georgia', 'Texas', 'Wyoming']"
              />
            </v-col>
            <v-col cols="3">
              <v-text-field
                v-model="formData.i2CAddress"
                label="i2c Address"
                readonly
              />
            </v-col>
            <v-col cols="3">
              <v-text-field
                v-model="formData.IP"
                label="IP Address"
                readonly
              />
            </v-col>
            <v-col cols="3">
              <v-text-field
                v-model="formData.i2cRegister"
                label="i2cRegister"
                readonly
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
                readonly
              />
            </v-col>
            <v-col cols="4">
              <v-text-field
                v-model="formData.MinimumFreeHeapSize"
                label="Minimum Free Heap Size"
                readonly
              />
            </v-col>
            <v-col cols="4">
              <v-text-field
                v-model="formData.FreeHeapSize"
                label="Free Heap Size"
                readonly
              />
            </v-col>
          </v-row>
          <v-row>
            <v-col cols="6">
              <v-text-field
                v-model="formData.Time"
                label="RTC current Time"
                readonly
              />
            </v-col>
          </v-row>

          <v-row>
            <v-col cols="6">
              <v-text-field
                v-model="formData.ResetReason"
                label="Reset Reason"
                readonly
              />
            </v-col>
          </v-row>
          <v-row>
            <v-col cols="3">
              <v-text-field
                v-model="formData.CompileDate"
                label="Value"
                readonly
              />
            </v-col>
            <v-col cols="2">
              <v-text-field
                v-model="formData.DeviceType"
                label="Compile Time"
                readonly
              />
            </v-col>
            <v-col cols="6">
              <v-text-field
                v-model="formData.IDFVersion"
                label="IDF Version"
                readonly
              />
            </v-col>
          </v-row>
          <v-row>
            <v-col cols="6">
              <v-text-field
                v-model.number="formData.index"

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
  "mdnsBase": "esp-home",
  "header": [
    {
      "text": 'Index',
      "align": 'center',
      "sortable": false,
      "value": 'index',
      "width": 15,
      "fixed": true,
    },
    {
      "text": 'Name',
      "align": 'center',
      "sortable": false,
      "value": 'Name',
      "width": 200,
    },
    {
      "text": 'order',
      "align": 'center',
      "sortable": false,
      "value": 'order',
      "width": 15,
    },
    {
      "text": 'Function',
      "align": 'center',
      "sortable": false,
      "value": 'function',
      "width": 80,
    },
    {
      "text": 'i2c Address',
      "align": 'center',
      "value": 'i2CAddress',
      "width": 100,
    },
    {
      "text": 'i2c Register',
      "align": 'center',
      "sortable": false,
      "value": 'i2cRegister',
      "width": 120,
    },
    {
      "text": 'Register index',
      "align": 'center',
      "value": 'registerName',
      "width": 80,
    },
    {
      "text": 'device Type',
      "align": 'center',
      "value": 'DeviceType',
      "width": 100,
    },
    {
      "text": 'Value',
      "align": 'center',
      "value": 'readValue',
      "width": 150,
    },
    {
      "text": 'Operation',
      "align": 'center',
      "sortable": false,
      "value": 'actions',
      "width": 110,
      "fixed": true,
    }, {
      "text": 'Param',
      "align": 'center',
      "sortable": false,
      "value": 'Param',
      "width": 110,
      "fixed": true,
    }],

  "name": 'I2CSchema',
  "props": {},
  "data": () => ({
    "formData": {
      id: '',
      registerName: '',
      IP: '',
      Param: '',
      i2CAddress: '',
      DeviceInstanceRevision: '',
      DEVICEINSTANCEFeatures: '',
      i2cRegister: '',
      order: '',
      Cores: '',
      SPIFlashChipMByte: "",
      EMBFlash: "",
      MinimumFreeHeapSize: "",
      Time: "",
      ResetReason: "",
      readValue: "",
      DeviceType: "",
      IDFVersion: "",
      FreeHeapSize: "",
      index: "cat",
      Comments: "",
    },
    "loading": false,
    "visible": false,
  }),
  "dataTest": () => ({
    formData: [{
      function: "Read",
      Name: "Read Who Am I",
      order: 0,
      Version: "KKKK",
      i2CAddress: 0x48,
      Param: {
        address: 72,
        index: 0,
        length: 2,
      },
      Comments: "comments",
      Cores: 1,
      deviceID: "unknown",
      IP: "192.168.100.101",
      ESP32ChipFeatures: 18,
      ESP32ChipRevision: 3,
      SPIFlashChipMByte: 4,
      EMBFlash: "External",
      i2cRegister: "11",
      registerName: "whoAmI",
      ResetReason: "Reset due to power on event",
      Time: "Thu Jan  1 18:54:53 1970",
      readValue: "none",
      DeviceType: "ADT7420",
      IDFVersion: "v4.4.1-585-g145c3cd000-dirty",
      MinimumFreeHeapSize: 198164,
      FreeHeapSize: 210936,
      interface: [
        "gpio",
        "i2c",
      ],
      "loading": false,
      "visible": false,
    },{
      function: "Read",
      Name: "Read Temp",
      order: 0,
      Version: "KKKK",
      i2CAddress: 0x48,
      Param: {
        address: 72,
        index: 11,
        length: 1,
      },
      Comments: "comments",
      Cores: 1,
      deviceID: "unknown",
      IP: "192.168.100.101",
      ESP32ChipFeatures: 18,
      ESP32ChipRevision: 3,
      SPIFlashChipMByte: 4,
      EMBFlash: "External",
      i2cRegister: "11",
      registerName: "whoAmI",
      ResetReason: "Reset due to power on event",
      Time: "Thu Jan  1 18:54:53 1970",
      readValue: "none",
      DeviceType: "ADT7420",
      IDFVersion: "v4.4.1-585-g145c3cd000-dirty",
      MinimumFreeHeapSize: 198164,
      FreeHeapSize: 210936,
      interface: [
        "gpio",
        "i2c",
      ],
      "loading": false,
      "visible": false,
    }],
  }),
  methods: {
    async getItem (id)
    {
    // this calls into the ap/device
      await getDevice(id).then((k) => {
        console.log(k.data)
      })
    },
    async add ()
    {
      await addDevice(this.formData)
      this.$emit('addSuccess')
    },
    async edit ()
    {
      await editDevice(this.formData)
      this.$emit('editSuccess')
    },
    async open (item)
    {
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
    async close ()
    {
      this.visible = false
      await this.$nextTick()
      Object.assign(this, this.$options.data.apply(this))
      this.$refs['form'].resetValidation()
      this.$refs['upload'].reset()
    },
    async submit ()
    {
      if (!this.$refs['form'].validate()) return
      try {
        this.loading = true
        this.formData.id ? await this.edit() : await this.add()
        await this.close()
      } finally {
        this.loading = false
      }
    }
    ,
  }
  ,
}
</script>

<style lang="scss">
</style>
`
