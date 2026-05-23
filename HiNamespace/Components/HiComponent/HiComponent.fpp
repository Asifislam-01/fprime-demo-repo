module HiNamespace {
    @ Hello World Tutorial Component
    active component HiComponent {

        @ Command to issue greeting with maximum length of 20 characters
        async command SAY_HI(
            greeting: string size 20 @< Greeting to repeat in the SayHiEvent event
        )

        @ Command to send a ping to HiEchoComponent via the greetingOut port
        async command SEND_TO_PEER(
            code: U32 @< Ping code forwarded to HiEchoComponent on greetingOut
        )

        @ Output port wired to HiEchoComponent.greetingIn in topology
        output port greetingOut: HiPing

        @ Greeting event with maximum greeting length of 20 characters
        event SayHiEvent(
            greeting: string size 20 @< Greeting supplied from the SAY_HI command
        ) severity activity high format "I say: {}"

        @ A count of the number of greetings issued
        telemetry GreetingCount: U32

        ###############################################################################
        # Standard AC Ports: Required for Channels, Events, Commands, and Parameters  #
        ###############################################################################
        @ Port for requesting the current time
        time get port timeCaller

        @ Enables command handling
        import Fw.Command

        @ Enables event handling
        import Fw.Event

        @ Enables telemetry channels handling
        import Fw.Channel

    }
}
