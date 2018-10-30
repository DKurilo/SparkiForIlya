import * as React from 'react';

import * as Types from '../types';

import { Sea } from '../Sea';
import { Comp } from '../Utils';

import './EnemySea.css';

export const EnemySea = (props:any) => {
  const clickHandler: (x: number) => (y: number) => (e:React.MouseEvent<HTMLImageElement>) => void = 
    x => y => e => props.clickEnemySea(x, y);
  const mouseEnterHandler: (x: number) => (y: number) => (e:React.MouseEvent<HTMLImageElement>) => void = 
    x => y => e => props.mouseEnter(x, y);
  const mouseLeaveHandler: (x: number) => (y: number) => (e:React.MouseEvent<HTMLImageElement>) => void = 
    x => y => e => props.mouseLeave(x, y);
  const sea: (s: Types.Props) => React.ReactElement<any> = s => <Sea
      click={clickHandler}
      enter={mouseEnterHandler}
      leave={mouseLeaveHandler}
      sea={s.enemySea}
      selected={s.selected}
      selectedPos={s.selectedPos}
    />;

  return (
    <div className="EnemySea">
      {Comp(sea).fold(props)}
    </div>
  );
};
