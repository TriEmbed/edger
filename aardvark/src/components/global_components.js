import Vue from 'vue'
import {DataTable} from './DataTable'
import {FormDrawer} from './FormDrawer'
import {Toast} from './Toast'

import {VFollowMenu, VImgUpload, VLoading} from './VImplements'
import {CssStyle} from './CssStyle'

Vue.component(DataTable.name, DataTable)
Vue.component(FormDrawer.name, FormDrawer)
Vue.component(VFollowMenu.name, VFollowMenu)
Vue.component(VImgUpload.name, VImgUpload)
Vue.component(VLoading.name, VLoading)
Vue.component(Toast.name, Toast)
Vue.component(CssStyle.name, CssStyle)
