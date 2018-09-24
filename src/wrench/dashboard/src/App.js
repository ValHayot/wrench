import React, { Component } from 'react';
const json = require('./output.json');

class App extends Component {
  render() {
    return (
      <div>
        {json.hello}
      </div>
    );
  }
}

export default App;
