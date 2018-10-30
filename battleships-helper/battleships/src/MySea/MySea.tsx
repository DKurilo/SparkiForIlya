import * as React from 'react';

import * as Types from '../types';

import { Sea } from '../Sea';
import { Comp } from '../Utils';

import './MySea.css';

export const MySea = (props:any) => {
  const clickHandler: (x: number) => (y: number) => (e:React.MouseEvent<HTMLImageElement>) => void = 
    x => y => e => props.clickMySea(x, y);
  const sea: (s: Types.Sea) => React.ReactElement<any> = s => <Sea click={clickHandler} sea={s} />;

  return (
    <div className="MySea">
      {Comp(sea).fold(props.mySea)}
    </div>
  );
};
