import React, { Component } from 'react';
const json = require('json-loader!./output.json');

class App extends Component {
  render() {
    return (
      <div>
        Hello
        {json}
      </div>
    );
  }
}

export default App;
