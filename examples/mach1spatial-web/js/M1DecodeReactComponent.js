'use strict';

class M1DecodeReactComponent extends React.Component {
    constructor(props) {
        super(props);
        this.state = {
            decoded: Array(),
            FRAMES_PER_SECOND: 60,
            m1Decode: null,
        };

        Mach1DecodeModule().then(function(m1DecodeModule) {
            var m1Decode = new(m1DecodeModule).Mach1Decode();
            m1Decode.setPlatformType(m1Decode.Mach1PlatformType.Mach1PlatformDefault);
            m1Decode.setDecodeAlgoType(m1Decode.Mach1DecodeAlgoType.Mach1DecodeAlgoSpatial);
            m1Decode.setFilterSpeed(0.95);

            this.state.m1Decode = m1Decode;
        }.bind(this));
    }

    componentDidMount() {
        this.timerID = setInterval(
            () => this.tick(),
            1000 / this.state.FRAMES_PER_SECOND
        );
    }

    componentWillUnmount() {
        clearInterval(this.timerID);
    }


    tick() {
        console.log('tick!');

        // check decode for loading
        if (this.state.m1Decode) {
            this.state.m1Decode.beginBuffer();
            let decoded = this.state.m1Decode.decode(0, 0, 0);
            this.state.m1Decode.endBuffer();

            this.setState({
                decoded: decoded
            });
        }
    }

    render() {
        return React.createElement('div', null, this.state.decoded.join(' '));
    }
}