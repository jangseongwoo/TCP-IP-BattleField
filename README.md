# TCP-IP-BattleField

> TCP/IP 소켓 통신을 이용해 실시간 대전 격투 기반의 게임을 네트워크 기능을 추가 시킨 프로젝트입니다

[![NPM Version][npm-image]][npm-url]
[![Build Status][travis-image]][travis-url]
[![Downloads Stats][npm-downloads]][npm-url]

TCP/IP 소켓 통신을 이용해 실시간 대전 격투 기반의 게임을 네트워크 기능을 추가 시킨 프로젝트입니다

![](../header.png)


## 게임 설명

- 게임 장르 : 액션 게임.

- 게임 방식 : 플레이 그라운드 위에서 사용자의 캐릭터가 차례로 나오는 NPC를 처치하는 게임. 

- 게임 목표 : 게임 내에 있는 NPC를 모두 다 처치하는 것.

- 게임 구성 : 사용자 캐릭터 , NPC , 플레이 그라운드, 아이템 , 하늘 ,건물 ,문, 원형 경기장(문을 열고 들어가면 있음)

- 게임 기능 :  상,하,좌,우 이동, 점프 ,주먹 공격 ,발차기 공격, 아이템 공격(종류 미정), 아이템 먹고 커지기 ,아이템 먹고 체력회복하기 ,건물 안으로 들어가기

## 구현 내용

1. 각 클라이언트에서 2명까지 클라이언트 접속가능(TCP)
2. 캐릭터 위에 HP 및 ID표시 Player Number표시(TCP)
3. 동시 타격 가능(TCP)
4. 각 클라이언트의 MAP전환 및 공유
5. 서버에서 각 클라이언트의 x ,y 위치 관리(TCP)
6. 업데이트 된 정보를 TCP 프로토콜으로 각 클라이언트에 실시간으로 전송(TCP).

## 게임 서버 구조와 데이터 흐름
<img align="center" src="battleField/images/server.png" width="600" height="550">


## 실행 방법

윈도우:

1. [git clone](https://github.com/jangseongwoo/TCP-IP-BattleField.git) 을 하세요.

2. 프로젝트 폴더 내  TCP-IP-BattleField/battleField/서버/서버.exe 를 실행시켜주세요.

3. 프로젝트 폴더 내 TCP-IP-BattleField/battleField/클라/클라.exe 를 실행시켜주세요.

```sh
npm install my-crazy-module --save
```



```sh
edit autoexec.bat
```

## 사용 예제

스크린 샷과 코드 예제를 통해 사용 방법을 자세히 설명합니다.

_더 많은 예제와 사용법은 [Wiki][wiki]를 참고하세요._

## 개발 환경 설정

모든 개발 의존성 설치 방법과 자동 테스트 슈트 실행 방법을 운영체제 별로 작성합니다.

```sh
make install
npm test
```

## 개발 환경

- 사용 OS : 윈도우7 64bit
- 개발 툴 : Visual Studio 2013 , OpenGL, Window API 라이브러리 사용.

## 정보

장성우 – [@facebook](https://www.facebook.com/profile.php?id=100007028118707&ref=bookmarks) – seongwoo.dev@gmail.com

MIT 라이센스를 준수하며 ``LICENSE``에서 자세한 정보를 확인할 수 있습니다.

[https://github.com/jangseongwoo/github-link](https://github.com/jangseongwoo/github-link)

<!-- Markdown link & img dfn's -->
[npm-image]: https://img.shields.io/npm/v/datadog-metrics.svg?style=flat-square
[npm-url]: https://npmjs.org/package/datadog-metrics
[npm-downloads]: https://img.shields.io/npm/dm/datadog-metrics.svg?style=flat-square
[travis-image]: https://img.shields.io/travis/dbader/node-datadog-metrics/master.svg?style=flat-square
[travis-url]: https://travis-ci.org/dbader/node-datadog-metrics
[wiki]: https://github.com/yourname/yourproject/wiki
