"use strict";

const React = require('react');
const ReactDOM = require('react-dom');

function container()
{
	let Btn = React.createClass({
		render: function () {
			return (
				<div>
					<button onClick={this.props.onClick(0)}>0</button>
					<button onClick={this.props.onClick(1)}>1</button>
					<button onClick={this.props.onClick(3)}>3</button>
					<button onClick={this.props.onClick(10)}>10</button>
					<button onClick={this.props.onClick(1000)}>all</button>
				</div>
			);
		}
	});

	let Row = React.createClass({
		parse: function (i) {
			return (
				<span key={i} className={this.props.text[i].desc}>
					{this.props.text[i].value}
				</span>
			);
		},
		render: function () {
			return (
				<div>
					{Object.keys(this.props.text).map(this.parse)}
				</div>
			);
		}
	});

	let Table = React.createClass({
		parse: function (i) {
			return (
				<Row text={this.props.values[i]} key={i}/>
			);
		},
		render: function () {
			return (
				<div>
					{Object.keys(this.props.values).map(this.parse)}
				</div>
			)
		}
	});

	let Container = React.createClass({
		getInitialState: function () {
			return {
				values: []
			};
		},
		componentDidMount: function () {
			this.btnClick(3)();
		},
		btnClick: function (count) {
			return () => {
				fetch('./get/all/count/' + count, {
					method: "GET",
					credentials: "same-origin",
					headers: {
						'Accept': 'application/json',
						'Content-Type': 'application/json'
					}
				}).then((msg) => {
					return msg.json();
				}).then((msg) => {
					let ans = [];

					for (let i in msg)
					{
						ans.push(msg[i].ans);
					}

					this.setState({
						values: ans
					});
				}).catch((err) => {
					console.warn('fetch /get/all/count/' + count);
					console.warn(err);
				});
			};
		},
		render: function () {
			return (
				<div>
					<Btn onClick={this.btnClick}/>
					<Table values={this.state.values}/>
				</div>
			);
		}
	});

	ReactDOM.render(<Container/>, document.getElementById('cont'))
}

module.exports = container;