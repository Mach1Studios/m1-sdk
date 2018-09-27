"use strict";

const React = require('react');
const ReactDOM = require('react-dom');
const fromData = require('./../tools').fromData;
const checkResponse = require('./../tools').checkResponse;

const delay = require('./../tools').delay;

const aq = function (initArr) {

	let arrOfQuestions = [];
	let maxTest = initArr.length;
	let regexp = new RegExp("\#\!\/step\/([0-9]*)");
	let firstTest = 0;
	let regExpExec = regexp.exec(location.hash);
	if (regExpExec && !isNaN(regExpExec[1]))
	{
		firstTest = Math.ceil(regExpExec[1]);
	}

	for (let i in arrOfQuestions)
	{
		if (typeof arrOfQuestions[i].f !== 'function')
		{
			arrOfQuestions[i].f = (x) => x;
		}
		if (typeof arrOfQuestions[i].change !== 'function')
		{
			arrOfQuestions[i].change = ()=>{};
		}
	}

	let Loader = React.createClass({
		render: function () {
			return (
				<div id="loadScreen" className={this.props.class}>
					<div className="assembly">
						<div className="tri"></div>
						<div className="tri"></div>
						<div className="tri"></div>
						<div className="tri"></div>
						<div className="tri"></div>
						<div className="tri"></div>
						<div className="tri"></div>
						<div className="tri"></div>
						<div className="tri"></div>
						<div className="tri"></div>
						<div className="tri"></div>
						<div className="tri"></div>
						<div className="tri"></div>
						<div className="tri"></div>
						<div className="tri"></div>
						<div className="tri"></div>
						<div className="tri"></div>
						<div className="tri"></div>
						<div className="hex"></div>
					</div>
				</div>
			);
		}
	});

	let Video = React.createClass({
		componentDidMount: function() {
			// this.refs.source.src = this.props.src;
		},
		render: function ()  {
			let cl = this.props.visible ? '' : 'hide';
			return (
				<div className={"player_wrapper " + cl}>
					<div id="status1">
						info: ...
					</div>
					<div className="player_container">
						<video id={this.props.id} className="video-js vjs-default-skin player" poster="assets/poster-2.jpg" crossOrigin="anonymous" controls>
							<source src={this.props.src} ref="source" type="video/webm" />
						</video>
						<div className="crosshair"></div>
					</div>
				</div>
			);
		}
	});

	let End = React.createClass({
		submit: function (event) {
			event.preventDefault();

			let val = this.refs.mail.value;

			this.props.submit(val);
		},
		render: function () {
			return (
				<div id="mail" className={this.props.end ? 'end' : ''}>
					<form onSubmit={this.submit}>
						<label>
							Mail
						</label>
						<input type="text" ref="mail"/>
						<input type="submit" value="Send"/>
					</form>
				</div>
			);
		}
	});

	let Hint = React.createClass({
		render: function () {
			return (
				<div className="hint">
					{this.props.text}
				</div>
			)
		}
	});

	let Error = React.createClass({
		render: function () {
			let cl = '';
			if (this.props.msg && this.props.msg.length > 0)
			{
				cl = 'error';
			}

			return (
				<div id="error" className={cl}>
					<div>
						{this.props.msg}
					</div>
				</div>
			);
		}
	});

	let Slider = React.createClass({
        getInitialState: function() {
        	return {
        		val: 0,
		        f_val: 0
			};
        },
		getValue: function() {
        	return this.state.f_val;
		},
		componentDidMount: function () {

		},
        change: function () {
            let v = $(this.refs.input).val();
            this.setState({
            	val: v,
            	f_val: this.props.qa.f(v).toFixed()
			});

            this.props.qa.change(this.props.qa.f(v).toFixed());
            // $(this.refs.val).html(v);
        },
		render: function() {
			return (
				<div key={30000 + this.props.key} className="range-slider">
					<input ref="input" onChange={this.change} className="range-slider__range"
						   type="range" value={this.state.val} min="0" max="1000" />
					<span ref="val" className="range-slider__value">{this.state.f_val}</span>
				</div>
			);
		}
	});

	let SoundDelayCheck = React.createClass({
		onClick: function () {
			this.props.qa.play();
		},
		getValue: function () {
			return this.refs.input.getValue();
		},
		render: function () {
			return (
				<div>
					<Slider qa={this.props.qa} key={3000 + this.props.key} ref="input"/>
					<button type="button" onClick={this.onClick}>Play</button>
				</div>
			);
		}
	});

	let OrientationCheck = React.createClass({
		getInitialState: function (){
			return {
				alpha: 0,
				beta: 0,
				time: 0
			};
		},
		componentDidMount: function () {
			let timer = setInterval(() => {
				// update must return {alpha, beta, time}
				let newState = this.props.qa.update();

				if (newState.isEnd)
				{
					clearInterval(timer);
				}
				else
				{
					this.setState(newState);
				}

			}, 50);
		},
		getValue: function () {
			return JSON.stringify({
				alpha: this.state.alpha,
				beta: this.state.beta,
				time: this.state.time
			});
		},
		render: function () {
			return (
				<div>
					<p>alpha: {this.state.alpha}</p>
					<p>beta: {this.state.beta}</p>
					<p>time: {this.state.time}ms</p>
				</div>
			);
		}
	});

	let Question = React.createClass({
		getValue: function () {
			switch(this.props.qa.type)
			{
				case "slider":
					return this.refs.input.getValue();
				case "delayCheck":
					return this.refs.input.getValue();
				case "orientationCheck":
					return this.refs.input.getValue();
				default:
					return this.refs.input.value;
            }
		},
		render: function () {
			let type = this.props.qa.type;

			switch(type)
			{
				case "slider":
					return (
						<div>
							<div>
								{this.props.qa.text}
							</div>
							<Slider qa={this.props.qa} key={3000 + this.props.key} ref="input"/>
						</div>
					);
				case "delayCheck":
					return (
						<div>
							<div>
								{this.props.qa.text}
							</div>
							<SoundDelayCheck qa={this.props.qa} key={3000 + this.props.key} ref="input"/>
						</div>
					);
				case "orientationCheck":
					return (
						<div>
							<div>
								{this.props.qa.text}
							</div>
							<OrientationCheck qa={this.props.qa} key={3000 + this.props.key} ref="input"/>
						</div>
					);
				default:
					return (
						<div>
							<div>
								{this.props.qa.text}
							</div>
							<input type="text" ref="input"/>
						</div>
					);
			}
		}
	});
	let Form = React.createClass({
		submit: function (event) {
			event.preventDefault();

			let val = this.refs.form.getValue();

			this.props.submit(val);
		},
		render: function () {
			return (
				<div>
					<form onSubmit={this.submit}>
						<Question key={300 + this.props.key} qa={this.props.qa} ref="form"/>
						<input type="submit" value="Submit"/>
					</form>
				</div>
			)
		}
	});

	let GlobalForm = React.createClass({
		getInitialState: function() {
			return {
				n: 0,
				qa: [],
				isLoad: 'load',
				error: "",
				isEnd: false,
				className: 'active',
				numOfTest: firstTest
			};
		},
		componentDidMount: function () {

			this.initPlayer(this.state.numOfTest);
		},
		initPlayer: function (i) {
			if (typeof initArr[i] === 'function')
			{
				initArr[i]((arr) => {
					arrOfQuestions = arr;

					this.setState({
						isLoad: '',
						qa: arrOfQuestions[0]
					});
				});
			}
			else
			{
				this.setState({
					error: 'Wrong init step values(' + location.hash + ', max test = ' + maxTest + ')'
				});
			}
		},
		onSubmit: function (val) {
			this.setState({
				isLoad: 'load'
			});

			fetch('/save/tmp', {
				method: "POST",
				credentials: "same-origin",
				body: fromData({
					desc: this.state.qa.desc,
					value: val,
					num: this.state.qa.number
				}),
				headers: {
					'Accept': 'application/json',
					'Content-Type': 'application/json'
				}
			}).then(checkResponse).then(() => {
				if (this.state.n >= arrOfQuestions.length - 1)
				{
					// end this test set
					this.state.qa.end();
					this.state.qa.change(0);

					this.setState({
						numOfTest: this.state.numOfTest + 1,
						n: 0
					});

					if (this.state.numOfTest < maxTest)
					{
						this.initPlayer(this.state.numOfTest);
					}
					else
					{
						this.setState({
							isLoad: '',
							isEnd: true
						});
					}
				}
				else
				{
					this.state.qa.change(0);

					this.setState({
						n: this.state.n + 1,
						qa: arrOfQuestions[this.state.n + 1],
						isLoad: ''
					});
				}
			}).catch((err) => {
				console.error(err);

				this.state.qa.end();

                this.setState({
                    error: err.message
                });
            });

		},
		submitMail: function (val) {
			this.setState({
				isLoad: 'load'
			});

			fetch('/save/end', {
				method: "POST",
				credentials: "same-origin",
				body: fromData({
					mail: val
				}),
				headers: {
					'Accept': 'application/json',
					'Content-Type': 'application/json'
				}
			}).then(checkResponse).then(()=> {
                this.setState({
                    isLoad: false,
                    className: 'inactive'
                });

                this.submitMail = ()=>{};

            }).catch((err)=> {
				console.error(err);

                this.setState({
                    error: err.message
                })
            });
		},
		render: function () {
			return (
			<div className={this.state.className}>
				<Video key={10 + this.state.numOfTest} id="videojs-panorama-player_first-second-test" src="./video/video.webm" visible={this.state.numOfTest < 2}/>
				<Video key={100 + this.state.numOfTest} id="videojs-panorama-player_fourth-test" src="./video/video.webm"  visible={this.state.numOfTest == 3} />
				<div id="status"></div>
				<Error msg={this.state.error}/>
				<Loader class={this.state.isLoad} />
				<div id="finishHim"><div>End</div></div>
				<End end={this.state.isEnd} submit={this.submitMail}/>
				<div id="container">
					<Form key={30 + this.state.n + 40000 * this.state.numOfTest} qa={this.state.qa} submit={this.onSubmit} />
					<Hint text={this.state.qa.hint}/>
				</div>
			</div>
			)
		}
	});

	ReactDOM.render(<GlobalForm />, $('#qa')[0]);
};

module.exports = aq;
/*
 <Form type={this.state.qa.type} text={this.state.qa.text} submit={this.onSubmit} className={this.state.isLoad} />
 <Error msg={this.state.error}/>
 <Hint text={this.state.qa.hint}/>
 <End end={this.state.isEnd} />
 */