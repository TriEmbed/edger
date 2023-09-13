import {AccountActions, AccountMutations, default as account} from './account'
import {default as setting, SettingMutations} from './setting'
import {default as runtime, RuntimeMutations} from './runtime'
import {default as esp, EspMutations} from "./esp"
import {AntMutations, default as ants} from "./ant"
import {default as temperature, TemperatueMutations} from "./temperature"

const modules = {
  account,
  setting,
  runtime,
  esp,
  ants,
  temperature,
}

export {
  modules as default,
  AccountActions,
  AccountMutations,
  SettingMutations,
  RuntimeMutations,
  EspMutations,
  AntMutations,
  TemperatueMutations,
}
