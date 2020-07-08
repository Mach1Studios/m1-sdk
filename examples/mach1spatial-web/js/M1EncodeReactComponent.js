'use strict';

class M1EncodeReactComponent extends React.Component {
    constructor(props) {
        super(props);
        this.state = {
            encodedCoeffs: Array(),
            FRAMES_PER_SECOND: 60,
            m1Encode: null,
        };


        Mach1EncodeModule().then(function(m1EncodeModule) {     
            var m1Encode = new(m1EncodeModule).Mach1Encode();

            this.state.m1Encode = m1Encode;
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

        // check encode for loading
        if (this.state.m1Encode) {
            this.state.m1Encode.setInputMode(this.state.m1Encode.Mach1EncodeInputModeType.Mach1EncodeInputModeMono);
            this.state.m1Encode.setOutputMode(this.state.m1Encode.Mach1EncodeOutputModeType.Mach1EncodeOutputModeM1Spatial);
            this.state.m1Encode.setAzimuth(0.0);
            this.state.m1Encode.setElevation(0.0);
            this.state.m1Encode.setDiverge(1.0);
            this.state.m1Encode.setStereoRotate(0.0);
            this.state.m1Encode.setStereoSpread(1.0);
            this.state.m1Encode.setAutoOrbit(true);
            this.state.m1Encode.setIsotropicEncode(true);

            this.state.m1Encode.generatePointResults();
            let encodedCoeffs = this.state.m1Encode.getGains();

            this.setState({
                encodedCoeffs: encodedCoeffs
            });
        }
    }

    render() {
        return React.createElement('div', null, this.state.encodedCoeffs.join(' '));
    }
}