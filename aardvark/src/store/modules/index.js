import {AccountActions, AccountMutations, default as account} from './account'
import {default as setting, SettingMutations} from './setting'
import {default as runtime, RuntimeMutations} from './runtime'
import {default as esp, EspMutations} from "./esp"
import {default as count} from "./count"

const modules = {
  account,
  setting,
  runtime,
  esp,
  count,
}

export {
  modules as default,
  AccountActions,
  AccountMutations,
  SettingMutations,
  RuntimeMutations,
  EspMutations,
}
