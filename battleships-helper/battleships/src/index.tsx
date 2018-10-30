import * as React from 'react';
import * as ReactDOM from 'react-dom';
import * as Types from './types';

import { EnemySea } from './EnemySea';
import { Footer } from './Footer';
import { MySea } from './MySea';
import { Settings } from './Settings';
import { Comp } from './Utils';

import './index.css';
import registerServiceWorker from './registerServiceWorker';

const initSea: (x: number, y: number) => Types.Sea = (x, y) => Array(y).fill(0).map(_ => Array(x).fill('empty'));

const init: (x: number, y: number) => Types.Game = (x, y) => ({
  enemy: 'sparki',
  enemySea: initSea(x, y),
  mode: 'config',
  mySea: initSea(x, y),
});

const clickMySea: (g:Types.Game) => (x: number, y: number) => void = g => (x, y) => {
  console.log(x, y);
  g.mySea[x][y] = 'ship';
  console.log(g.mySea);
  render(g);
};

const clickEnemySea: (g:Types.Game) => (x: number, y: number, g: Types.Game) => void = g => (x, y) => {
  console.log(x, y);
  g.enemySea[x][y] = 'hit';
  render(g);
};

const config: (g: Types.Game) => () => void = g => () => {
  g.mode = 'config';
  render(g);
};

const toggleEnemy: (g: Types.Game) => () => void = g => () => {
  g.enemy = g.enemy === 'sparki' ? 'human' : 'sparki';
  render(g);
};

const play: (g: Types.Game) => () => void = g => () => {
  g.mode = 'play';
  render(g);
};

const reset: () => void = () => {
  render(init(10, 10));
};

const title: (g:Types.Game) => React.ReactElement<any> = g => <h1>Human vs {g.enemy === 'sparki' ? 'Sparki' : 'human'}!</h1>;

const settings: (g:Types.Game) => React.ReactElement<any> = g => 
<Settings
  switchToConfig={config(g)}
  switchToPlay={play(g)}
  toggleEnemy={toggleEnemy(g)}
  reset={reset}
  mode={g.mode}
  enemy={g.enemy} 
/>;

const mySea: (g:Types.Game) => React.ReactElement<any> = g => 
<MySea 
  clickMySea={clickMySea(g)}
  mySea={g.mySea}
/>;

const enemySea: (g:Types.Game) => React.ReactElement<any> = g => 
<EnemySea 
  clickEnemySea={clickEnemySea(g)}
  enemySea={g.enemySea}
/>;

const footer: (g:Types.Game) => React.ReactElement<any> = g => <Footer />;

const render = (game: Types.Game) => {
  ReactDOM.render(
    Comp(title).concat(Comp(settings)).concat(Comp(mySea)).concat(Comp(enemySea)).concat(Comp(footer)).fold(game),
    document.getElementById('root') as HTMLElement
  );
}
reset();

registerServiceWorker();
