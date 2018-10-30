import * as React from 'react';
import * as Types from '../types';

import { Comp } from '../Utils';

import './Sea.css';

import './empty.png';
import './hit.png';
import './miss.png';
import './ship.png';

const SeaLine = (props: any) => {
  const cell: (i: number) => Types.Box = i => Comp(g =>
    <div 
      className={'Cell ' + g.line[i] + 
                 (g.selectedPos === i || g.selected ? ' light' : '') +
                 (g.selectedPos === i && g.selected ? ' current' : '') }
      onClick={g.click(i)}
      onMouseEnter={g.enter(i)}
      onMouseLeave={g.leave(i)}
    />
  );
  const seaLine: (s: Types.SeaLine) => Types.Box = 
    s => s.reduce((acc, i, y) => acc.concat(cell(y)), Comp(g => <React.Fragment />));

  return seaLine(props.line).fold(props);
}

export const Sea = (props:any) => {
  const sea: (s: Types.Sea) => Types.Box = s => s.reduce((acc, i, x) => acc.concat(Comp(g =>
    <div className="SeaLine"><SeaLine 
      click={g.click(x)}
      enter={g.enter(x)}
      leave={g.leave(x)}
      line={g.sea[x]}
      selected={g.selected && g.selectedPos && g.selectedPos.x === x}
      selectedPos={g.selected && g.selectedPos ? g.selectedPos.y : undefined}
    /></div>
  )), Comp(g => <React.Fragment />));

  return (
    <div className="Sea">{sea(props.sea).fold(props)}</div>
  );
};
