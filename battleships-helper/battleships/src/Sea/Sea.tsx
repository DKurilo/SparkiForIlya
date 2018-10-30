import * as React from 'react';
import * as Types from '../types';

import { Comp } from '../Utils';

import './Sea.css';

const SeaLine = (props: any) => {
  const cell: (i: number) => Types.Box = i => Comp(g =>
    <div className="Cell" onClick={g.click(i)}>{g.line[i]}</div>
  );
  const seaLine: (s: Types.SeaLine) => Types.Box = 
    s => s.reduce((acc, i, y) => acc.concat(cell(y)), Comp(g => <React.Fragment />));

  return seaLine(props.line).fold(props);
}

export const Sea = (props:any) => {
  const sea: (s: Types.Sea) => Types.Box = s => s.reduce((acc, i, x) => acc.concat(Comp(g =>
    <div className="SeaLine">
      <SeaLine click={g.click(x)} line={g.sea[x]} />
    </div>
  )), Comp(g => <React.Fragment />));

  return (
    <div className="Sea">
      {sea(props.sea).fold(props)}
    </div>
  );
};


