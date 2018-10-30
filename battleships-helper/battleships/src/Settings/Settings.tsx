import * as React from 'react';

import './Settings.css';

export const Settings = (props:any) => {
  const configClickHandler: (e:React.MouseEvent<HTMLImageElement>) => void = e => props.switchToConfig();
  const playClickHandler: (e:React.MouseEvent<HTMLImageElement>) => void = e => props.switchToPlay();
  const resetClickHandler: (e:React.MouseEvent<HTMLImageElement>) => void = e => props.reset();
  const enemyClickHandler: (e:React.MouseEvent<HTMLImageElement>) => void = e => props.toggleEnemy();

  return (
    <div className="Settings">
      <h2>Current mode: {props.mode}, current enemy: {props.enemy}</h2>
      <div onClick={configClickHandler}>Config</div>
      <div onClick={playClickHandler}>Play</div>
      <div onClick={resetClickHandler}>Reset</div>
      <div onClick={enemyClickHandler}>Toggle Enemy</div>
    </div>
  );
};
