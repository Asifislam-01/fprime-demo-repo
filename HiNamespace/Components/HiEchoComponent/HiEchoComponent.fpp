module HiNamespace {

    @ Receives HiPing port calls from HiComponent (port-to-port demo)
    active component HiEchoComponent {

        @ Input port connected from HiComponent.greetingOut in topology
        async input port greetingIn: HiPing

        @ Raised when a ping arrives on greetingIn
        event MessageReceived(
            code: U32 @< Ping code received from HiComponent
        ) severity activity high format "HiEcho received ping code {}"

        @ Count of pings received on greetingIn
        telemetry MessageCount: U32

        ###############################################################################
        # Standard AC Ports: Required for Channels, Events, and Telemetry             #
        ###############################################################################
        @ Port for requesting the current time
        time get port timeCaller

        @ Enables event handling
        import Fw.Event

        @ Enables telemetry channels handling
        import Fw.Channel

    }
}
