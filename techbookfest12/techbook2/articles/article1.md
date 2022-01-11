
# Flowでクリーンアーキテクチャーを最適化する

## はじめに

弊社の一部チームでは、モバイルアプリ開発にクリーンアーキテクチャーを採用しています。2019年に執筆した[ACCESSテックブック](https://access-company.github.io/techbookfest/techbookfest7/ACCESS_techbook_ebook_20190922.pdf)では、クリーンアーキテクチャーを使ってみた感想を紹介しました。

クリーンアーキテクチャーには、プロジェクトが大きくなって新しい人が介入しても、基本の設計ポリシーが保たれるという利点があります。しかし、モジュールやクラスの多さに伴ってデータの流れがわかりづらくなるため、開発時間やレビュー時間、学習コストの面では、クリーンアーキテクチャー採用前と比べてさほど変わらないという感想でした。

本書では、それから数年経って、どのような改善を施しているかを紹介します。

一言で言えば、Unidirectional Data Flow（以下、単方向データフロー）をより強固にします。クリーンアーキテクチャーの思想を守りつつ、データの流れをわかりやすくするためです。本書の前半では、Kotlin Coroutines Flow（以下、Flow）を使い、簡単な例を掲載しながらそれについて説明します。

そして後半では、単方向データフローのロジックをKotlin Multiplatform Mobile（以下、KMM）のフレームワークとして実装し、UI部分をSwiftで実装するiOSアプリの例を掲載します。KMMはAndroid/iOSの両方がターゲットですが、Androidはロジックと同じKotlinで労せずFlowを使えます。一方iOSはKotlinのFlowをSwiftで受け取る必要があるのです。そこをどうすればよいのか、お楽しみの終盤になっています。

なお、クリーンアーキテクチャーの概要は、本書では触れていません。ACCESSテックブックの第1章、もしくは数ある書籍や検索をご参照ください。

## 単方向データフロー

最初に、単方向データフローについて詳しく説明します。

2010年代のモバイルアプリ開発では、Two-way Data Binding（以下、双方向データバインディング）が主流でした。特にAndroidは、Data BindingやView Bindingが公式から出ていたため、その傾向は顕著でした。iOSも、Rx系を使って擬似的or結果的に双方向を実現する手法が多く使われてきました。

ですが、2020年代に入るとSwiftUI/CombineやJetpack Composeなどの導入が進み、Web FrontendのFlux系に似た単方向データフローが浸透し始めています。

### 双方向データバインディングとは

単方向データフローと対の概念である双方向データバインディングとは、ある値がユーザー→Viewの入力に連動し、また通信結果などにも連動している状態のことです。値はViewModelに置かれることが多いです。

![双方向データバインディングのデータの流れ](./images/1_1.jpg)

このように各境界が双方向に繋がっており、UseCaseの変更がViewに、Viewの変更がUseCaseに伝播しやすく、責務を分割しながらコードを整理できるのが特徴です。

見かけのコード量は少なくできますが、各境界でのデータ型の変換や、ビューパーツごとにバインディングするための実装が必要で、全体のコード量は減りづらい性質があります。

### 単方向データフローとは

単方向データフローは、ユーザーがViewに与えた変更をActionとして受け取り、値は直接更新しません。 Actionはロジックを経てStateを更新し、StateがViewに描画されるというサイクルの関係です。

![単方向データフローのデータの流れ](./images/1_2.jpg)

単方向データフローでは、データは常に同じ方向へ流れます。逆流は禁止されています。

ActionはViewからの命令、StateはActionの結果、そしてViewはStateの結果です。

### 単方向にするメリットとは

一見すると、双方向データバインディングのほうが階層化されており理解しやすそうです。しかし、単方向化には色々とメリットがあります。

1. 単一化/カプセル化...MutableなStateを1箇所にまとめられる。双方向の場合、最新の状態をどこに持たせるかが曖昧で、散らばりやすい
2. 共有...1つのStateを複数の子Viewで使いまわしやすい。双方向の場合、状態の共有はViewModelの共有やView間のデータ渡しでなんとかする傾向があり、実装が複雑化する
3. 分離...図1.1と図1.2を比較すると、接している矢印の数が少ないほうが切り出しや拡張が容易で、デバッグやテストをしやすく、変化による副作用も少なく抑えられる

もちろんメリットだけでなく、単方向化のデメリットも存在します。

1. 単一化/カプセル化...MutableなStateが1箇所なので、適用できるデザインパターンが限られる
2. 共有...Viewのライフサイクルに合わせて必要ないStateの監視を止めないと、メモリが枯渇したりランタイムエラーを起こすことがある
3. 分離...Viewのコード量や階層が増える。パーツごとに描画用のVariableと入力用のListenerを必ず置く必要がある。

MVVMや双方向なMVCに慣れた人は単方向の思想をアンチパターンと感じたり、プロジェクトごとに実装スタイルの差が出やすいのも特徴です。

### Androidアプリの変化

Androidアプリの場合、双方向が主流だった時代は[BindingAdapter](https://developer.android.com/topic/libraries/data-binding/two-way?hl=ja#two-way-custom-attrs)がよく使われ、推奨アーキテクチャがMVVMなことからも、双方向データバインディングは暗黙的に推奨されていました。

これはアプリ規模が大きくなるにつれ、アプリ独自のバインディングが多数実装されることを意味します。やりすぎるとブラックボックス化が起きたり、Android初心者からは学習ハードルが高いと感じられていた部分です。

また、接続がアノテーション任せな上に双方向なので、デバッグやバグ調査がしづらい問題もありました。

2021年7月、待望のJetpack Composeが登場し、[単方向データフロー](https://developer.android.com/jetpack/compose/architecture?hl=ja#udf-compose)が公式推奨されました。これを導入すれば、UIからStateを直接変えられなくなるため、先述の複雑性からある程度解放されます。

また、デメリットの1つであるライフサイクル問題も、remember宣言子によってStateの生存期間をViewライフサイクルに合わせることで回避できます。

### iOSアプリの変化

iOSアプリは、MVCやMVC+VMのようなアーキテクチャがよく使われてきました。

しかし、Rx系をふんだんに使わないとあっという間にControllerが肥大化したり、外部のフレームワークに頼らざるをえないのが積年の問題でした。一部をRouterに切り出したり、VIPERを導入したりなどの模索が続きました。

2019年、Swift UI/Combineが登場し、こちらも単方向データフローが公式サポートのスタート地点に立ちました。非公式ですが[TCA](https://qiita.com/tonionagauzzi/items/a97a7303b546e515fc9b)も登場し、着々と置き換えが進んでいます。

## クリーンアーキテクチャーと単方向データフロー

続いて、クリーンアーキテクチャーと単方向データフローの親和性に関する考察です。

次の図は、私たちのチームで当初使っていたクリーンアーキテクチャーの設計図です。

![クリーンアーキテクチャーの依存関係](./images/1_3.jpg)

domainから他へインターフェースを提供し、「この通りに実装して結果をください」（to backend, data, sensor）、「Presenter型のオブジェクトを作ってくれれば結果を代入します」（to UI）とし、domainから他への依存を無くすのが、クリーンアーキテクチャーの特徴です。

この図だと、データはViewModelからInteractor、Presenterへと流れ、そしてViewModelに帰って来るので、その部分はキッチリ単方向データフローを守っています。

しかし、ViewModelに置かれた状態は、Viewからもdomainからも参照でき、更新も可能です。人によってはInteractorやRepositoryに状態を持たせるかもしれません。

つまり、中途半端に単方向も双方向も取り入れているので、プロジェクトが大きくなればデータの流れが複雑化し、保守が困難になるリスクを抱えています。

冒頭で述べたとおり、私たちがすべき改善は、どちらかに舵を回しきることだと考えています。単方向データフローがより強固になるよう、データの流れをわかりやすくしていきます。

### PresenterをStateに置き換え

Presenterを採用していると、最新の状態をどこに置くかが曖昧になります。

また、ActionからViewに戻るまでのルートを自由に決められるので、私の参画してる案件でもApplicationやAppDelegateがPresenterを継承して、データの行き先がよくわからなくなってるロジックが多少ありました。

そのPresenterを無くし、UIからdomainに渡すものをAction、domainからUIにはStateの更新を通知するように変えてみます。

![単方向データフローを適用したUIとdomain間の依存関係](./images/1_4.jpg)

図1.2っぽくなりました。ところが、このままではクリーンアーキテクチャーに反しています。Notify Updateのところで、UIがdomainに依存しているからです。

これは、UI側からStateをSubscribeすることで回避可能です。

![単方向データフローとDIPを適用したUIとdomain間の依存関係](./images/1_5.jpg)

さて、データの流れはViewとViewModelの間にもあります。

ViewModelからViewへは、依存こそありませんが、データは流れます。つまり、ViewとViewModel間のデータの流れはまだ双方向です。このままでは、ViewModelのテストがしづらいです。

### 結局ViewModelは必要なのか

![ViewModelを廃止したUIとdomain間の依存関係](./images/1_6.jpg)

思いきってViewModelを取ってみました。すると、Viewに以下の責務が集中します。

1. 画面の表示
2. Stateの監視
3. Stateから画面パーツ向けの型変換
4. 画面の更新
5. 画面遷移

これなら、2と3をViewModelとして分けてた方がまだ読みやすいでしょうし、テストのしづらさに至っては悪くなっています。

では、Viewからのイベント発火で直接interactorを呼び、ViewModelは上記2と3に専念させればどうでしょうか。

![Viewから直接Actionを発火する場合のUIとdomain間の依存関係](./images/1_7.jpg)

スッキリしましたね。

この場合のViewModelは、SubscribeしたデータをView向けに変換してViewに送り出す役割ですが、iOSの[TCA](https://qiita.com/tonionagauzzi/items/a97a7303b546e515fc9b)ではViewStore、[Unio](https://github.com/cats-oss/Unio)ではViewStreamと呼ばれています。

ViewModelという呼び方が適切かどうかは諸説ありつつ、コンバーターとしての役割はViewから切り離したほうがよいと私は考えます。

## Kotlin Coroutines Flow

ここまではOSを限定せず記述しましたが、ここからは実際にAndroidアプリでFlowを使う例を紹介します。

Flowとは、Kotlin Coroutinesの新しい非同期処理用ライブラリです。RxやPromiseに似た記述ができ、コールドストリームであることが特徴です。

### コールドストリーム

Subscribeされたら初めて動きだす、Observableなストリームです。ストリームとは、データを連続して送り出す型を言います。

上司が来たら初めて働き出すぐうたら社員をイメージしてみてください。上司がいる間は、状況の変化をちゃんと逐次報告します。1人の上司にのみ報告するというのも特徴です。そして、上司がいなくなったらすぐに自分から働くのをやめます。

社員だとどうかなと思うコールドな働き方ですが、プログラムとして、必要ないときに働かないのは実は強力な利点なのです。必要なときだけリソースを食い、不要になったら開放してくれるからです。

しかし、1人の上司にしか報告できない点は、Observerが2つ登録されると、2つのコールドストリームが必要なことを意味します。これはメモリ効率を考えればいまいちなところです。

### ホットストリーム

反対の型がホットストリームです。こちらもObservableですが、Subscriberがいなくても値を発行し、データを送り出します。Publisherと呼ばれることが多くあります。

こちらは上司が来る前から働き出す頑張り屋さんです。上司がいる間、状況の変化を逐次報告するのはコールドと同じですが、複数の上司がいても同じ報告を1人で請け負います。そして、上司が止めてくれるまで、いなくても働き続けるのです。

ちゃんと止めてあげないと必要ないときも働き続けてしまうのですが、Observerがいくつ登録されても、使うリソースが変わらないのは利点です。

これを応用すると、1つのコールドストリームを受信し、複数のSubscriberに送信させるという中継地点の役割も担えます。

### Kotlinのストリーム事情

元々Kotlinには、Channelというホットストリームが存在していました。しかし、suspendの非同期処理をシーケンシャルに繋げたい場合、コールドストリームのほうが望ましく、それは遅れて登場したFlowを待つ必要がありました。

実際にFlowが登場すると、非同期処理を直感的に実装でき、安全で習得しやすく使いやすいからと、次々と移行が進んでいます。

### Flowの基本的な使い方

Androidアプリで、100ms毎に0から100までカウントする処理を、Flowを使って双方向データバインディングで実装してみましょう。

#### CounterUseCase.kt

```Kotlin:CounterUseCase.kt
class CounterUseCase {
    suspend fun countStream(): Flow<Int> = flow {
        repeat(100) { count ->
            delay(100)
            emit(count) // 0 1 2 3 4 ... 99
        }
    }
}
```

クリーンアーキテクチャーの図1.3でいうInteractorの部分が、無加工のデータを非同期に送ります。

#### CounterViewModel.kt

```Kotlin:CounterViewModel.kt
class CounterViewModel: ViewModel() {
    val showing = MutableLiveData<String>()

    fun showCountEvenNumbersSquared() {
        viewModelScope.launch(Dispatchers.Main) {
            useCase.countStream()
                .drop(1)
                .filter { it % 2 == 0 }
                .map { (it * it).toString() }
                .take(5)
                .collect { count.value = it } // 4 16 36 64 100
        }
    }
}
```

ViewModelでは、それを表示向けに加工します。

Presenterの代わりにFlowのObserverがおり、データが流れてきたらdropやtakeなどFlowの様々なオペレーターを使い、加工や除外を行います。

#### MainActivity.kt

```
counterViewModel.showCountEvenNumbersSquared()
```

#### activity_main.xml

```Kotlin:activity_main.xml
<TextView
    android:layout_width="wrap_content"
    android:layout_height="wrap_content"
    android:text="@{viewModel.showing}" />
```

Viewは、計算実行と表示を担当します。

データの流れは、図1.1と同等になります。`showing`が最新の値を持つ変数で、それをどこからでも更新できるのが不安材料です。

では、単方向データフローにするとどう変わるでしょうか。

#### CounterState.kt

```Kotlin:CounterState.kt
sealed class CounterState {
    data class Init(val count: Int = 0) : CounterState()
    data class Success(val count: Int) : CounterState()
    data class Error(val exception: Exception) : CounterState()
}
```

図1.2に適合するため、Stateクラスを作成します。

ただのカウンターにSuccessやErrorを持たせるのは若干大袈裟ですが、何かの処理を行い結果を返す場合の基本構成です。

#### CounterUseCase.kt

```Kotlin:CounterUseCase.kt
class CounterUseCase {
    private val mutableState = MutableStateFlow(CounterState.Init() as CounterState)
    val state: StateFlow<CounterState> = mutableState
    suspend fun countStream() {
        repeat(100) { count ->
            delay(100)
            mutableState.emit(count) // 0 1 2 3 4 ... 99
        }
    }
}
```

Interactorが、無加工のデータを非同期に送ります。

先ほどと違うのは、FlowインスタンスをStateクラス型で外部へ公開している点です。

StateFlowは、Flowを継承した状態管理用のホットストリームなFlowで、LiveDataに似たものです。詳しくは後述しますが、Stateを導入する上で最も容易な手段なので採用しています。

データ操作はMutableStateFlowでないと行えませんが、データ更新をどこからでも行えるのはリスクのある設計なので、MutableStateFlow型は非公開にします。そのため、CounterUseCaseのみがStateの更新が可能です。

#### CounterViewModel.kt

```Kotlin:CounterViewModel.kt
class CounterViewModel: ViewModel(
    useCase: CounterUseCase
) {
    private val mutableShowing = MutableStateFlow(String)
    val showing: StateFlow<String> = mutableShowing

    init {
        useCase.state
            .drop(1)
            .filter { it is Success && it.list % 2 == 0 }
            .map { it is Success && (it * it).toString() }
            .take(5)
            .onEach { new ->
                (new as? Success)?.count?.let {
                    mutableShowing.value = it // 4 16 36 64 100
                }
            }
            .launchIn(viewModelScope)
    }

    fun counter() {
        viewModelScope.launch(Dispatchers.Main) {
            useCase.countStream()
        }
    }
}
```

ViewModelが、それを表示向けに加工します。

先ほどと違うのは、ここでもStateFlowを外部へ公開している点です。今まで説明した依存関係に従い、公開先はViewです。

#### MainActivity.kt

```
counterViewModel.showCountEvenNumbersSquared()
```

#### activity_main.xml

```Kotlin:activity_main.xml
<TextView
    android:layout_width="wrap_content"
    android:layout_height="wrap_content"
    android:text="@{viewModel.showing}" />
```

Viewは、計算実行と表示を担当します。

データの流れは、図1.7と同等になります。`state`や`showing`は、適切なコンポーネントへのみ更新を許しています。

サンプルコードに出てきたViewModelScopeとDispatcherは、Flowを使う上では理解を欠かせないキーワードですが、本書では省略します。

### SharedFlowとStateFlow

さて、先ほど出てきたStateFlow、そして異なる性質を持つSharedFlowの2つについて説明します。

Flowでの値の更新は、上記UseCaseの`flow { ... }`ラムダ式中でしかできません。つまりViewModel側では値を更新できず、また`.value`のように値の参照もできません。

subscribeしてる数だけ`flow { ... }`ラムダ式が呼ばれてしまうのも特徴です。

それでは状態保持や処理リソースの節約には向いてないからと、ホットストリームなFlowとして登場したのが、ここで紹介するSharedFlowとStateFlowです。

#### SharedFlowとは

複数箇所でのsubscribeでデータや状態を共有できるFlowで、処理リソースの節約に向いています。

単なるFlowと違う点は以下です。

```Kotlin
sharedFlow.onEach {
    println("1")
}.launchIn(scope)

sharedFlow.onEach {
    println("2")
}.launchIn(scope)
```

* このように複数subscribeしてても`flow { ... }`ラムダ式側は1回しか呼ばれない
* 処理開始/subscribe終了のタイミングを選択できるが、これを適切に指定しないとsubscribeされ続ける
  * `LiveData`に変換し、`observe`引数に`LifecycleOwner`を設定すれば、表示中だけのsubscribeも可能
* 色々高機能
  * replay: subscribeした瞬間、過去のn回の値を受信する
  * buffer: 複数subscribeかつ処理に時間がかかるとき、1回目に行われた処理をバッファリングして、2回目以降を早くしてくれる

1つだけのFlowインスタンスを全ての場所で参照し、監視は必要な間だけ動作させたり、永続的に監視しつつ`replay`で最後に発行された10個を常に監視するなどのトリックが可能です。
  
#### StateFlowとは

状態保持に特化したSharedFlowです。LiveDataに似ていますが、LiveDataはAndroid、SharedFlowはKotlinのフレームワークです。とはいえ、LiveDataの後継機能として使うこともできます。

* 初期値が必須
* 現在の状態を`.value`で受け取れる
* MutableStateFlowを使えば、`.value`への代入も可能
  * その際Coroutines Scopeは不要
* `launchIn`で直近の値を1つ受信する
* 同じ値の代入は受信しない
* waitなどを挟まず連続して値が変更されたとき、最後の値しか受信しない
  * つまり**状態**が**保持**されないと**状態変化**とみなされない

`sharedFlow`では、Viewを開いたタイミングでflowがサーバー通信などの処理中なら、直近の値をどう表示するのかで迷います。しかし、`stateFlow`では`.value`に直近の値がキャッシュされているので、迷わずに済みます。

#### 初期化方法

初期化は、`MutableSharedFlow`、`MutableStateFlow`を使って行うか、`shareIn`、`stateIn`を使ってFlowから変換します。`shareIn`は`sharedFlow`インスタンスを、`stateIn`は`stateFlow`インスタンスを返します。

#### 注意点

関数の戻り値で`shareIn`や`stateIn`をしてはなりません。そうすると、関数の呼び出しごとに新しい`SharedFlow`または`StateFlow`が作成され、リソースの無駄使いになります。

また、ユーザーIDのような入力値を持つFlowは、異なる入力値で複数回開始した場合、`subscribe`が共有されていると新旧IDが混じって誤動作するリスクがあります。`shareIn`や`stateIn`で安易に共有してはならないパターンです。

#### 開始/終了タイミングの指定

`shareIn`や`stateIn`は`flow { ... }`ラムダ式をホットストリーム化するので、開始/終了タイミングを指定できます。`SharingStarted`オプションで、

1. `shareIn`や`stateIn`の際に開始して永続的に有効な`Eagerly`
2. subscribeが行われた際に開始して永続的に有効な`Lazily`
3. subscribeが行われた際に開始してsubscribeされている間だけ有効な`WhileSubscribed`

を選択することができます。

#### 結局どれがいいのか

Flow、SharedFlow、StateFlow、そしてSharedFlowかStateFlowの場合は開始/終了タイミングを選択できますが、どれを選択すべきかは、場合によって異なります。大事なのは、要件に応じたものを適切に使い分けることです。

1. subscribe場所の結果に狂いが生じないこと
2. リソースの無駄使いにならないこと

を念頭に置いて判断しましょう。

### FlowとLiveData、Rxとの比較

Androidアプリでは、LiveDataやRxJavaからFlowへの置き換えが少しずつ進んでいます。ですが、Flowの何が良いのかわからないまま周りを気にして使っている方や、新しいからなんとなく使っている方も少なくないと思います。

そのような方のために、従来手法と比較し、Flowを使うメリットを見つけましょう。

#### 従来手法の問題点

まず、Rx系を使う上で避けては通れない問題が、OSのライフサイクルへの適合やオーバーヘッド対策を盛り込むこと、あるいはそれらの考慮抜けによるバグです。

LiveDataはAndroid Jetpackの一部なので、Androidのライフサイクルやメモリ/キャッシュとの親和性は抜群です。初心者のぶっつけ実装でもその類のバグが起きにくいことで、LiveDataの優秀さを実感できると思います。もちろん、`LifecycleOwner`の指定間違いなど致命的なものはあります。

一方、次々と起こる状態変化の`subscribe`や、Model→ViewModelのデータ変換部分に注目すると、そこは複雑化やバグの温床を抱えたままです。

### Flow移行のメリット

#### 1. 構造化された並行性

FlowにあってLiveDataにない主な1つは、`map`の再計算やデータ変換を`flowOn`により簡単に他スレッドへ投げ、結果だけをUI側で受け取れることです。

コールドストリームなので、次々と状態変化が起きても無駄なくスレッドを使い、破棄もしてくれます。

#### 2. さまざまな演算子

`map`、`filter`、`onEach`、`reduce`など、LiveDataには無い多くの演算子で効率的なデータ変換を標準サポートしてくれます。

#### 3. テスタビリティ

テストのしやすさもFlowに軍配が上がります。

LiveData＋Coroutinesのテストはピタゴラスイッチのようなもので、他のテストの干渉を受けないよう上手く作らないとFailが起きたり、`observe`のチェーンになってしまいます。

Flowでは`flowOn`する際の`dispatcher`に`TestCoroutineDispatcher`を使い、`runBlockingTest`で走らせることで、必ず決まった順序でテストが可能で、無用意な影響を排除できます。

その順序をチェックする`collectIndexed`など、便利なオペレーターも揃っています。

一部だけモックをDIすることも容易です。

#### まとめ

LiveDataからFlowに移行すると、無駄のない非同期処理が書け、いろんな演算子も使え、テストも捗ります。

ただし、FlowはAndroidではなくKotlinの機能なので、Androidで使う場合`flowWithLifecycle`などの導入を忘れないようにしましょう。

そして、FlowがLiveDataやRxより必ず便利とは限りません。作ろうとしてるものが何か次第です。上記のメリットを生かせると判断すれば完全Flowで、確信がなければUI部分はLiveData、ロジック部分はFlowを使うのが無難でしょう。

AndroidのFlowには`asLiveData`という変換オペレーターもあります。

## Kotlin Coroutines FlowをSwiftでobserveする

ここまでFlowについて説明しましたが、例えばUI以外をKotlin Multiplatform Mobile（以下、KMM）で実装し、UIはSwiftで実装するiOSアプリの場合、**KotlinのFlowはSwiftでも受け取れるのか？**という疑問が起きます。

結論としては受け取れますし、Flowの強みである各種オペレーターも、そのままとはいきませんがSwiftの事情に合わせる形で容易に利用できます。

では、それを確かめましょう。

### 環境構築
* 必須要件：[Android Studio 4.2以上 / Xcode 11.3以上](https://kotlinlang.org/docs/kmm-setup.html) / [macOS Mojave 10.14.4](https://developer.apple.com/support/xcode/)
* 私の環境：Android Studio Arctic Fox 2020.3.1 / Xcode 13.1 (13A1030d) / macOS Big Sur 11.5.2

せっかくなので、KMMアプリを1から作る手順を載せます。

1. Android Studioで、Android Studio→Preferences→Pluginsで、Kotlin Multiplatform Mobileをインストール
2. Android Studioで、File→New→New Project、KMM Applicationを選択
3. パッケージ名など入れる
4. Add sample tests for Shared moduleにチェック入れ（本記事ではテストコード書かないですが今後のため）、iOS framework distributionはRegular frameworkとする
5. フォルダ階層をAndroidからProjectに変える
6. iOSのデバッグ設定をEdit Configurationsから行う
7. Execution Targetを好みのSimulatorに指定
8. Simulatorが出てこない場合はXcode→Window→Devices and Simulators→Simulatorsタブの＋ボタンで追加

これで準備完了です。

ちなみにGradle SettingでJDK 1.8だとビルド時に`Android Gradle plugin requires Java 11 to run. You are currently using Java 1.8.`というエラーになるので、Android Studio→Preferences→Build, Execution, Deployment→Build Tools→GradleでGradle JDKを11以上に設定します。
![Gradle JDKの設定](./images/1_17.jpg)

最後に、`shared/build.gradle.kts`にcommonで使う依存関係を追加します。

```Kotlin:shared/build.gradle.kts
val commonMain by getting {
    dependencies {
        implementation("org.jetbrains.kotlin:kotlin-stdlib:1.6.0")
        implementation(
            "org.jetbrains.kotlinx:kotlinx-coroutines-core:1.5.2-native-mt"){
            version {
                strictly("1.5.2-native-mt")
            }
        }
    }
}
```

`native-mt`を付与しないと、iOSからのCoroutines呼び出しで強制終了が発生します。

後々依存関係を追加する際にバージョンの不整合を起こさないため、`strictly`も設定します。

### 実装

この章で作るのは、**赤ちゃんに見せるアルバムアプリ**です。

1歳の娘が、自分やいとこの写真をスマホで見るのが大好きなのです。しかし、OS標準のフォトアプリで写真を見せてると、手を伸ばして触れていろんな操作をしてしまいます。

たとえば削除してゴミ箱を空にしたり、共有を開きメールを開いて画像を誰かに送信なんてことがあると困ります。実際タッチパネルが敏感すぎて予期せぬことが意外と起きます。

なので、アルバムアプリの仕様は次のようにします。

1. 15秒ごとに異なる画像をランダムに表示する
1. ユーザー入力は一切受け付けない
1. Homeボタンを押さないとアプリを抜けられない

設計はこうします。Repositoryより先はありません。
![](./images/1_18.jpg)

サーバーやデータ保存まで作り込むと本書の記事が長くなり伝えたい本質がどこかわからなくなるので、極シンプルなプログラム仕様にします。

1. InteractorとRepositoryは、15秒ごとに[0-4]の数字をランダムでStateに反映
3. ViewModelは、StateをSubscribeし、その数字に応じた画像名をViewにPublish
4. Viewは、`@Published`変数をSubscribeし、その画像をアセットから見つけて表示

その通りコードを書いていきます。Kotlinのソースコードは全てsharedのcommonMain階層下です。

#### SwiftStateFlow.kt

まずは、本記事のメインテーマとなるStateFlowをSwiftで使うための`SwiftStateFlow`を実装します。

```Kotlin:SwiftStateFlow.kt
package com.vitantonio.nagauzzi.babyalbum

import kotlinx.coroutines.CoroutineScope
import kotlinx.coroutines.Dispatchers
import kotlinx.coroutines.Job
import kotlinx.coroutines.flow.Flow
import kotlinx.coroutines.flow.StateFlow
import kotlinx.coroutines.flow.launchIn
import kotlinx.coroutines.flow.onEach

class SwiftStateFlow<T>(private val kotlinStateFlow: StateFlow<T>) : Flow<T>
        by kotlinStateFlow {
    val value = kotlinStateFlow.value
    var job: Job? = null

    fun observe(continuation: ((T) -> Unit)) {
        kotlinStateFlow.onEach {
            continuation(it)
        }.launchIn(
            CoroutineScope(Dispatchers.Main + Job().also { job = it })
        )
    }

    fun close() {
        job?.cancel()
        job = null
    }
}
```

呼び元は`continuation`クロージャの引数でデータを受け取ります。

FlowでなくStateFlowを選択したのは、前回と同じ値を連続発行しないためのチェックをInteractorに設ける予定だからです。

ここで、Flowの豊富なオペレーターを駆使すればいいのでは？と思われた方は鋭いですね！ぜひ後述の**CombineとFlowを連動してより使いやすくする**というところを見てください。

呼び元が破棄されるときに`close()`を呼ぶと中断もできます。実際、後述のViewModelの`deinit`で呼んでます。

#### PublishNumber.kt (Interactor)

次に、KMM側のInteractor/Repository/Stateを実装します。

```Kotlin:PublishNumber.kt
package com.vitantonio.nagauzzi.babyalbum.domain.interactor

import com.vitantonio.nagauzzi.babyalbum.SwiftStateFlow
import com.vitantonio.nagauzzi.babyalbum.domain.repository.NumberRepository
import com.vitantonio.nagauzzi.babyalbum.domain.state.NumberState
import kotlinx.coroutines.*
import kotlinx.coroutines.flow.*

class PublishNumber(
    private val repository: NumberRepository
) {
    private val mutableState = MutableStateFlow(NumberState.Init(0) as NumberState)
    private val swiftMutableState = SwiftStateFlow(mutableState)

    val state: StateFlow<NumberState>
        get() = mutableState
    val swiftState: SwiftStateFlow<NumberState>
        get() = swiftMutableState

    fun execute(min: Int, max: Int, times: Int) =
        CoroutineScope(Dispatchers.Default).launch {
            repeat(times) {
                delay(15000)
                mutableState.value = NumberState.Updated(
                    repository.getChangedRandom(min, max,
                        before = state.value.number)
                )
            }
        }
}
```

#### NumberRepository.kt (Repository)

```Kotlin:NumberRepository.kt
package com.vitantonio.nagauzzi.babyalbum.domain.repository

class NumberRepository {
    fun getChangedRandom(min: Int, max: Int, before: Int): Int {
        val random = (min..max).random()
        return if (random == before) {
             getChangedRandom(min, max, before)
         } else {
             random
         }
    }
}
```

#### NumberState.kt (State)

```Kotlin:NumberState.kt
package com.vitantonio.nagauzzi.babyalbum.domain.state

sealed class NumberState(open val number: Int) {
    data class Init(override val number: Int) : NumberState(number)
    data class Updated(override val number: Int) : NumberState(number)
}
```

この程度ならInteractor/Repository/Stateに分けるメリットがあまり無いのですが、行く行くはサーバーから画像をダウンロードしてByteArrayか何かでInteractorに渡し、StateはInit/Success/Failureに分かれることを想定しているので、今のうちにと分けてしまいました。

#### AlbumViewModel.swift (ViewModel)

さて、ここからはSwift側です。

KMMのプロジェクトを作るだけでSwiftUIがデフォルト生成されているので、まずViewModelを作ります。

```Swift:AlbumViewModel.swift
import shared

class AlbumViewModel: ObservableObject {
    private let photos = (1...5).map { "BabyImage\($0)" }
    private let interactor: PublishNumber
    
    @Published var photoName: String

    init(interactor: PublishNumber) {
        self.interactor = interactor
        self.photoName = self.photos[0]
        interactor.swiftState.observe { newState in
            if 0...4 ~= newState!.number {
                self.photoName = self.photos[Int(newState!.number)]
            } else {
                fatalError("newNumber isn't supported number")
            }
        }
    }
    
    deinit {
        self.interactor.swiftState.close()
    }
}
```

先頭の`import shared`で、KMMで作った`SwiftStateFlow`やInteractorにアクセスできるわけです。始めれば難しくないですよね、KMM！

そして、`observe`がFlowを監視・データ受信する部分です。画像名に変えて`photoName`に入れると、@Published が付いているのでViewの更新トリガーの役割を果たしてくれます。

`photos`が配列で画像名を持っていますが、実際の画像は`BabyImage1`〜`BabyImage5`をassetに登録済みです。

![](./images/1_19.jpg)

#### ContentView.swift (ViewModel)

最後に、ContentViewをデフォルトから変更します。これが設計図のViewにあたります。

```Swift:ContentView.swift
 import shared
 
 struct ContentView: View {
-    let greet = Greeting().greeting()
-
+    @ObservedObject var viewModel: AlbumViewModel
+    let interactor: PublishNumber
+    
+    init() {
+        self.interactor = PublishNumber(repository: NumberRepository())
+        self.viewModel = AlbumViewModel(interactor: interactor)
+    }
+    
     var body: some View {
-        Text(greet)
+        ZStack {
+            Color.black
+                .ignoresSafeArea()
+            Image(viewModel.photoName)
+                .resizable()
+                .aspectRatio(contentMode: .fill)
+        }.task {
+            self.interactor.execute(min: 0, max: 4, times: 100)
+        }
     }
 }
```

### プレビューと実行

ここで、SwiftUIのプレビューが動いていないことに気づきました。

Android Studio側でRunするとビルドできるのですが、Xcode側ではまだxcframeworkをインポートしてないので、そのビルドエラーによってプレビューやデバッグ、エラー調査などができないのです。

Xcodeでそれらができたら開発効率が上がるので、直しましょう。

iosAppのTARGETS→Build Phases→Link Binary With Librariesで、`./BabyAlbum/shared/build/bin/iosX64/debugFramework/shared.framework`を追加します。
![](./images/1_20.jpg)
![](./images/1_21.jpg)

ただし、とりあえずビルドを通したいだけなので、CPUアーキテクチャーによっては上記出力先のframeworkではダメなこともあります。

いちいち設定を弄るのは面倒なので、実際に動かすためのビルドはAndroid Studioからがよいでしょう。

これでひとまず仕様は満たせました。

![](./images/1_22.jpg)

プレビューもされてるし、実行すると15秒毎に違う画像に変わります。これはSwiftStateFlowが15秒毎のFlowのデータ送出をSwiftにうまく伝えているからですね。

iPadに入れて赤ちゃんに見せてあげたいと思います😊

## Combineを使って改善する

さて、ここまでで満足かというと、**本当に作りたかったものはこれじゃない**感があります。

SwiftStateFlowが`onEach`を逐次横流ししてるだけじゃん！みたいな。

Flowの強みである様々なオペレーター（`map`、`filter`、`onEach`、`reduce`など）を使いたいのです。

とはいえFlowと同じオペレーターをSwiftで1から実装するのは大変だし、shared.hに定義されてるKotlinx_coroutines_core系の型を使ってFlowの扉をこじ開けるのもちょっと根気が必要そうです。

そもそも、無理してFlowに合わせるより、SwiftならSwiftらしい実装をしたいですよね。

そこで、Swift標準の非同期フレームワークである**Combine**の出番です！

AlbumViewModelを以下のように書き直します。

#### AlbumViewModel.swift（ViewModel）

```Swift:AlbumViewModel.swift
import Combine
import shared

class AlbumViewModel: ObservableObject {
    private let photos = (1...5).map { "BabyImage\($0)" }
    private let interactor: PublishNumber
    private var cancellable: Cancellable?
    
    @Published var photoName: String

    init(interactor: PublishNumber) {
        self.interactor = interactor
        self.photoName = self.photos[0]
        self.cancellable = NumberStatePublisher(stateFlow: interactor.swiftState)
            .map { newValue in
                self.photos[Int(newValue.number)]
            }
            .assign(to: \.photoName, on: self)
    }
    
    deinit {
        self.cancellable?.cancel()
    }
}

public struct NumberStatePublisher: Publisher {
    public typealias Output = NumberState
    public typealias Failure = Never
    
    private let stateFlow: SwiftStateFlow<Output>
    
    public init(stateFlow: SwiftStateFlow<Output>) {
        self.stateFlow = stateFlow
    }

    public func receive<S: Subscriber>(subscriber: S)
            where S.Input == Output, S.Failure == Failure {
        let subscription = NumberStateSubscription(stateFlow: stateFlow, subscriber: subscriber)
        subscriber.receive(subscription: subscription)
    }
}

final class NumberStateSubscription<S: Subscriber>: Subscription
        where S.Input == NumberState, S.Failure == Never {
    private let stateFlow: SwiftStateFlow<S.Input>
    private var subscriber: S?

    public init(stateFlow: SwiftStateFlow<S.Input>, subscriber: S) {
        self.stateFlow = stateFlow
        self.subscriber = subscriber
      
        stateFlow.observe { newValue in
            _ = subscriber.receive(newValue!)
        }
    }
  
    func cancel() {
        subscriber = nil
        stateFlow.close()
    }

    func request(_ demand: Subscribers.Demand) {}
}
```

Combineの仕組みは省略しますが、必要なPublisherとSubscriptionを作りました。

最も肝心なのは`func startSubscribe()`の部分です。受け取った数字を`map`で文字列加工して、直接`photoName`にバインドしています。この数行だけでロジックから流れてきたデータをView向きに加工して再描画まで行っていることを考えれば、とてもシンプルですよね。もちろん`map`以外のCombineオペレーターも使用可能です。

これが標準フレームワークのオペレーターを使用できる大きな利点です。改良前の`onEach`を横流しするだけのViewModelもコード量は少ないのですが、複雑な実装になると`observe`部分が膨れ上がり、じわじわと痛めつけられるでしょう。

また、Publisherを何かに`assign`するとCancellableオブジェクトが取れるので、その参照を取っておいてViewModelが解放される際に監視を止めることも可能です。改良前は直接Interactorを介してSwiftStateの`cancel`を呼ぶ必要があり、複数箇所で監視している場合の考慮ができていませんでした。

## まとめ

まとめる力などもう残ってない（笑）2022/1/11(火)に続きを書きます。
