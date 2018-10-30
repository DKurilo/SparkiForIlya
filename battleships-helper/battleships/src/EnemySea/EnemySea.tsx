import * as React from 'react';

import './EnemySea.css';

export const EnemySea = (props:any) => {
  const parseIntSafe: (x:any) => number = x => parseInt('' + x, 10) || 0;
  const clickHandler: ((e:React.MouseEvent<HTMLImageElement>) => void ) = 
    e => props.clickEnemySea(
      parseIntSafe(e.currentTarget.dataset.x), 
      parseIntSafe(e.currentTarget.dataset.y)
    );

  return (
    <div className="EnemySea">
      <div onClick={clickHandler} data-x={1} data-y={2}>{props.enemySea[1][2]}</div>
    </div>
  );
};
