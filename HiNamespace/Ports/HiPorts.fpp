module HiNamespace {

  @ Port for passing a greeting code between Hi components
  port HiPing(
    code: U32 @< Value sent from HiComponent to HiEchoComponent
  )

}
