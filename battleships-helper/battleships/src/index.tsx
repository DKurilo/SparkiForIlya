import * as React from 'react';
import * as ReactDOM from 'react-dom';
import * as Types from './types';

import Battleships from './Battleships';
import './index.css';
import registerServiceWorker from './registerServiceWorker';

const Comp: (g:Types.Component) => Types.Box = g => ({
  concat: (other: Types.Box) =>
    Comp(x=>(
      <React.Fragment>
        {g(x)}
        {other.fold(x)}
      </React.Fragment>
    )),
  contramap: (f:(p:Types.Props) => Types.Props) => Comp(x => g(f(x))),
  fold: g,
  map: (f:(e:Types.Element) => Types.Element) => Comp(x => f(g(x))),
});

const initSea: (x: number, y: number) => Types.Sea = (x, y) => Array(y).fill(Array(x).fill('empty'));

const init: (x: number, y: number) => Types.Game = (x, y) => ({
  enemySea: initSea(x, y),
  mode: 'config',
  mySea: initSea(x, y),
});

const clickMySea: ((x: number, y: number, g: Types.Game) => void) = (x, y, g) => {
  console.log(x, y);
  g.mode = 'play';
  render(g);
};

// const clickEnemySea: ((x: number, y: number, g: Types.Game) => void) = (x, y, g) => {
//   console.log(x, y);
//   g.mode = 'play';
//   render(g);
// };

// const config: ((g: Types.Game) => void) = (g) => {
//   g.mode = 'config';
//   render(g);
// };

// const play: ((g: Types.Game) => void) = (g) => {
//   g.mode = 'config';
//   render(g);
// };

const Battlefield: (s:Types.Game) => React.ReactElement<any> = s => <Battleships clickSea={clickMySea} mySea={s.mode} state={s} />;
const TestEl: (s:Types.Game) => React.ReactElement<any> = s => <h1>{s.enemySea[0][0]}</h1>;

const render = (state: any) => {
  ReactDOM.render(
    Comp(Battlefield).concat(Comp(TestEl)).fold(state),
    document.getElementById('root') as HTMLElement
  );
}
render(init(10, 10));

registerServiceWorker();
