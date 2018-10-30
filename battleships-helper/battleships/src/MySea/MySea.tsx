import * as React from 'react';

import './MySea.css';

export const MySea = (props:any) => {
  const parseIntSafe: (x:any) => number = x => parseInt('' + x, 10) || 0;
  const clickHandler: ((e:React.MouseEvent<HTMLImageElement>) => void ) = 
    e => props.clickMySea(
      parseIntSafe(e.currentTarget.dataset.x), 
      parseIntSafe(e.currentTarget.dataset.y)
    );

  return (
    <div className="MySea">
      <div onClick={clickHandler} data-x={4} data-y={4}>{props.mySea[4][4]}</div>
    </div>
  );
};
