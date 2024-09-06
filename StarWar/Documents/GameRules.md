# 游戏背景

近日，一颗无生命体的新星球在M星系被发现。M星系的九大文明种族都想对其进行管辖和开发，按照星际联盟条约规定，九大文明需各派一名代表来到联盟议会，通过一场模拟战斗来争夺新星球的归属权。

# 游戏目标

本场游戏，玩家作为各个文明的代表，要在`12x12` 的方格地图上抢占地盘。玩家需利用`行动点`占领更多的格子，并利用`地雷`破坏其他玩家的占领行动。最终占领格子数最多的玩家获胜。

# 游戏初始设定

初始每位玩家拥有`100`金币，可用于竞拍控制权和购买道具。每轮每位玩家会获得`4`个`行动点`和`1`个`普通地雷`。`行动点`决定可占领的格子数。格子中可埋入``地雷``，踩到会发生爆炸，对已占领格造成破坏。

# 游戏流程

游戏共`4`轮。每轮分为自由阶段，控制阶段，埋雷阶段，占领阶段和购买阶段。

## 自由阶段

玩家可自由交流，分享信息并制定策略。

> 请注意，`行动点`，`金币`和`道具`都**不能**进行交易。

## 控制阶段

玩家用``金币``竞拍当轮的``控制权``，价高者得。获得``控制权``的玩家将在当轮的埋雷阶段，占领阶段和购买阶段决定所有玩家的行动顺序。若出价最高的玩家无力支付等额的金币，则重新开始竞拍，该玩家将失去整场游戏的控制权竞拍资格。

## 埋雷阶段

拥有当轮``控制权``的玩家可任意决定所有玩家的埋雷顺序。玩家依次独自埋雷。当轮获得的``普通地雷``**必须埋入**，``特殊地雷``不限；只能在未占领的格子中埋雷；同一个格子中可埋入多个地雷；埋雷位置**不公开**。随后，玩家可自由交流，再进入占领阶段。

## 占领阶段

拥有当轮``控制权``的玩家可任意决定所有玩家的占领顺序。玩家依次在方格地图上**公开**进行占领行动。玩家可从任意未占领的格子上开始占领，每占领一格消耗一个``行动点``。已被占领的格子不能重复占领；当轮占领路径必须**连贯且不能回头**。每占领一格，将进行判定。若格子中没有地雷，则占领成功，格子归该玩家所有，随后玩家可继续进行占领或主动结束占领行动；若格子中有地雷，则地雷爆炸，本轮该玩家已占领的格子**全部**回归无主状态，玩家被迫结束占领行动。无论主动与被动，占领行动结束时，未用完的``行动点``都会累积到下一轮。位于同一格子的地雷被踩中会**同时爆炸**并执行相应效果。爆炸后的地雷将被移除，每个雷的拥有者会秘密获得`10`金币。

## 购买阶段

拥有当轮``控制权``的玩家可任意决定所有玩家购买顺序。玩家依次进入商店购买道具，每种道具**数量有限**。进店的玩家可查看所有道具的当前余量，每轮最多只能购买**一个道具**。

> 注意，第四轮占领阶段后没有购买阶段，直接进入最终结算。

# 最终结算

首先结算每位玩家占领的格子数，再结算每位玩家占领格的最大连接数。格子上下左右相邻视为连接，斜角相邻不视为连接。最大连接数第一的玩家占领格额外加`3`，第二的玩家占领格额外加`1`。最终根据玩家占领的格子数进行排名，如格子数相同，则比较剩余金币，金币更多的玩家排名靠前。

## 道具信息

### 特殊地雷

#### 矩阵地雷
`20`金币 数量`5`
该地雷正常可覆盖`2x2`的格子，若其中包含已占领格，则**不会**覆盖已占领格。矩阵地雷**任意**一个格子被踩中都会发生爆炸，爆炸后被**整体移除**，地雷拥有者获得`10`金币。

> 注意，该地雷爆炸时，除被踩中的格子外，其他覆盖区域的地雷**不会**被引爆。

#### 追击地雷

`30`金币 数量`4`
该地雷埋下后，玩家需选择一个**已占领格**作为``目标点``。该地雷爆炸后，立即向``目标点``追加投放一个地雷，将``目标点``引爆，相应的占领格立即回归无主状态。

> 注意，目标点本身并无地雷。

#### 孢子地雷
`40`金币 数量`3`
该地雷埋下后视为``普通地雷``。在下一轮埋雷阶段开始时，若该地雷仍未爆炸也未被移除，则会发生扩散。围绕该地雷的`8`个格子中，每个**未占领格**都将出现一颗``普通地雷``，且**全部**归该埋雷玩家所有。扩散只会发生**一轮**。

#### 净化地雷
`50`金币 数量`2`
该地雷爆炸后，将清空以该地雷为中心的`8`个格子。八个格子中所有地雷都会被**移除**（矩阵地雷任意一个格子在其中就会被移除，追击地雷会直接被移除而不引爆目标点），被移除地雷的拥有者不会获得金币，且八个格子中的已占领格都会**回归无主状态**。

### 反地雷装备

#### 排雷手
`30`金币 数量`5`
拥有该道具的玩家踩到**仅有**普通地雷的格子时，会被告知可使用排雷手。该玩家可消耗`1`名排雷手将所有普通地雷移除，地雷拥有者不会获得金币。

#### 冲锋车
`40`金币 数量`4`
玩家开始自己的占领行动前，可派遣冲锋车探路。冲锋车可从任意未占领格开始行动，其行动点与该玩家当下的行动点相等，**且须全部用完**。每行至一格会进行判定，踩到地雷会发生爆炸，冲锋车被消耗，地雷拥有者会获得金币。若冲锋车行动点用完仍未踩到地雷，下一轮可继续使用；若同时拥有多辆冲锋车，可在同一占领阶段连续使用。

#### 透视仪
`50`金币 数量`3`
若玩家当轮占领第一格时未踩中地雷，则之后每占领一格前，都可使用透视仪。选择当前格的上下左右其中一格，公开查看该格中的地雷数量。该道具无使用次数限制。

#### 扫雷机器人
`60`金币 数量`2`
拥有该道具的玩家在自己的占领行动前，可公开指定一行或一列，该行/列上的所有``普通地雷``都将被扫雷机器人移除（特殊地雷除外），地雷拥有者不会获得金币。该道具使用一次即被消耗。

#### 探测雷达
`70`金币 数量`1`
可在任意轮次轮到自己的埋雷阶段时使用，可独自查看当前所有的地雷位置及数量，但无法查看地雷种类及所属玩家，该道具使用一次即被消耗。