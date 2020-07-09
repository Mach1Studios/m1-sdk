'use strict';

class M1DecodePositionalReactComponent extends React.Component {
    constructor(props) {
        super(props);
        this.state = {
            decoded: Array(),
            distance: 0.0,
            FRAMES_PER_SECOND: 60,
            m1DecodePositional: null,
        };

        Mach1DecodePositionalModule().then(function(m1DecodePositionalModule) {
            var m1DecodePositional = new(m1DecodePositionalModule).Mach1DecodePositional();
            m1DecodePositional.setDecodeAlgoType(m1DecodePositional.Mach1DecodeAlgoType.Mach1DecodeAlgoSpatial);
            m1DecodePositional.setPlatformType(m1DecodePositional.Mach1PlatformType.Mach1PlatformDefault);
            m1DecodePositional.setUseAttenuation(true);
            m1DecodePositional.setAttenuationCurve(1.0);
            m1DecodePositional.setUsePlaneCalculation(false);

            this.state.m1DecodePositional = m1DecodePositional;
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
        if (this.state.m1DecodePositional) {
            this.state.m1DecodePositional.setListenerPosition(0, 0, 0);
            this.state.m1DecodePositional.setListenerRotation(0, 0, 0);
            this.state.m1DecodePositional.setDecoderAlgoPosition(0, 0, 10);
            this.state.m1DecodePositional.setDecoderAlgoRotation(0, 0, 0);
            this.state.m1DecodePositional.setDecoderAlgoScale(1, 1, 1);

            let distance = this.state.m1DecodePositional.getDist();
            let decoded = this.state.m1DecodePositional.getCoefficients();

            this.setState({
                decoded: decoded,
                distance: distance,
            });
        }
    }

    render() {
        return React.createElement('div', null, this.state.decoded.join(' '), ' | distance: ', this.state.distance);
    }
}