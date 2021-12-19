= よくないレビューの例

本章では、よくないレビューの例をいくつか挙げます。ただし、本書で取り上げるよくないレビューとは、人間関係を悪くしたり、不必要な悩む時間を生んだり、レビューに出す人のメンタル状態やモチベーションを下げるようなレビューのことを指します。

== 審査員形式

エマちゃんは、チームに配属されて1ヶ月の新人さんです。勉強期間も終わって徐々に商用アプリ開発を任されるようになり、一生懸命やってプルリクエストを出しました。

//talklist{
//talk[chara_ema]{
実装終わったのでレビューお願いします！
//}
//talk[chara_maria]{
アメさん、見てあげれる？
//}
//talk[chara_ame]{
ちょっと今忙しくて…。ムチさんに見てもらってください。
//}
//talk[chara_ema]{
わかりました！
//}
//talk[chara_ema]{
（アメさんがよかったなぁ…。ムチさんちょっと怖いんだよな…）
//}
//}

こういうシーン、みなさんのプロジェクトでも心あたりあるかもしれません。なぜ、エマちゃんはアメさんにはお願いしやすくて、ムチさんにはお願いしにくいのでしょうか。

おや、そう言ってる間にムチさんからレビューコメントが返ってきたようです。

//list[00001][＠Muchi commented]{
NGです。レビュアーやテスターに伝わる説明を書く努力をしてください。
冗長コードも多すぎます。既存の全体設計を事前に見なかったのでしょうか。
ここはif-elseの判定が仕様と逆ですが、わざと破壊しようとしていますか？
//}

〜翌日〜

//talklist{
//talk[chara_ema]{
（なんか体が重いなぁ。今日は午前休しよう…）
//}
//talk[chara_maria]{
あれっ、エマちゃんは今日どうしたの？
//}
//talk[chara_ame]{
午前休って連絡きてましたよ。
//}
//talk[chara_maria]{
あ、ホントだ。体が重いって言ってるのが気になるね。心当たりは？
//}
//talk[chara_ame]{
昨日、ムチさんのレビューを受けて、遅くまで修正していたようです。
//}
//talk[chara_maria]{
なるほど。何かあったのかな。レビュー記録を見てみよう。
//}
//talk[chara_maria]{
うーん、厳しい指摘が多いみたいだなぁ。スケジュールの心配もあるし、ムチさんに話を聞いてみよう。
//}
//}

エマちゃんが午前休を取ってしまったのは、甘えやメンタルが弱いからでしょうか。そう思われては気の毒ですよね。エマちゃんは自分なりによいものを作ろうと一生懸命取り組んでいたはずです。

マリアさんはムチさんを呼んで、コメントの真意を聞いてみました。

//talklist{
//talk[chara_maria]{
ムチさん、エマさんの実装ですが、何か大きな問題がありますか？
//}
//talk[chara_muchi]{
いえ、そこまでは。
//}
//talk[chara_maria]{
レビューの指摘が結構重たいようなんですが。
//}
//talk[chara_muchi]{
あれは、少し勉強不足を感じたので、自分で考えて成長してほしいと期待しました。
//}
//}

ムチさんの言い分もよくわかります。甘いだけではやっていけないのがプロの世界ですからね。

ですが、エマちゃんはやる気を削がれ、望まない理由で有休まで消費してしまいました。これはエマちゃんと会社の双方にとって、非常にもったいないことですよね。

では、このレビューはどうすればよかったのでしょうか。

//note[■審査員形式のレビューとは]{
・複数のレビュアーが辛口審査員のように振る舞う

・発表者のコードを攻撃的に批判する
//}

今回は、批評者はムチさんだけですが、エマちゃんが努力不足だったりよく考えずにやっている、さらに悪意があって間違えたなどと批判するかのようなコメントをしています。

これは発表者のメンタルを壊し、チームには鬱憤とやる気の低下が残るのみなのです。

//talklist{
//talk[chara_maria]{
（とはいえ、ムチさんは元々そういうスタイルの人で、悪気はないから注意するのもなぁ…。そうだ、アメさんにも見てもらおう）
//}
//talk[chara_maria]{
アメさん、このレビュー指摘だけど、アメさんからもフォローしてくれない？
//}
//talk[chara_ame]{
はーい。
//}
//talk[chara_ame]{
（ふむふむ、なるほど。エマちゃんはこれに悩んでたのか…。私ならこう書くかな）
//}
//}

//list[00002][@Ame commented]{
ご対応ありがとうございます。実装箇所は合ってますが、いくつかお願いしたいことをMust/Wantで伝えますね。

Must: if-elseを仕様通りに直す
Want: 類似実装の共通化、誤りを未然に防ぐための単体テスト作成

説明文はフォーマットを使うとよいと思いますよ。
わからないところは過去のPRを参考にしてみてください。
//}

//talklist{
//talk[chara_ema]{
おはようございます〜。今朝はすみませんでした〜。
//}
//talk[chara_ame]{
おはよう。大丈夫？ムチさんの指摘がちょっと怖かった？
//}
//talk[chara_ema]{
あっ、そうじゃないんです。でも、直しててなんか辛くなっちゃいました…。
//}
//talk[chara_ame]{
そういうときは、抱え込まず相談してね。私も最初、ムチさんにあんな感じでしごかれたのよね。
//}
//talk[chara_ema]{
えっ、そうなんですか！意外です。
//}
//talk[chara_ame]{
私のほうが後から入ったのもあるけど、あの人、新人だから厳しいんじゃなくて、誰に対してもそうなのよね。
//}
//talk[chara_ame]{
でも、ムチさんはきっとこう伝えたかったと思うよ。
//}
//}

それからアメさんは、「私ならこう書く」と考えたコメント通りエマちゃんに伝えました。それを聞いて、エマちゃんはやる気を取り戻しました。

アメさんのコメントは審査ではなく、実装者に寄り添うような提案をしていますね。良かった部分は認め、改善点は必須と任意を分けてヒントを出すに留め、実装者自身が考えて学ぶ余地も残しています。

エマちゃんは1年目で1つ1つの達成を自信に変えている段階なので、それを後退させるフィードバックはチーム力の後退にも繋がります。

気持ちのよいフィードバックとは、必ずしも全員一緒ではないのですが、コミュニケーションを重ねる中で、適切なバランスを見つけていきましょう。

== 試験形式

//talklist{
//talk[chara_ema]{
実装終わったけど今日はアメさん不在だし、期限が近いからムチさんに出さなきゃ…。今度こそ何も言われなければいいな！
//}
//}

//list[00001][＠Muchi commented]{
NGです。このAPIが例外を吐いたときの考慮がされていません。
typoがいくつかあるし、インデントも不揃いです。全体的に不合格。
//}

//talklist{
//talk[chara_ema]{
ぐすん…不合格なんて…。コンビニスイーツで元気になろう…。
//}
//}

ある日の朝会の後、マリアさんはエマちゃんを呼び止め、こう問いかけました。

//talklist{
//talk[chara_maria]{
エマちゃんも入って1ヶ月経ったね。何か困ってることある？
//}
//talk[chara_ema]{
私はどうすれば1人前になれるでしょうか…。
//}
//talk[chara_maria]{
というと？
//}
//talk[chara_ema]{
こないだムチさんにレビューで不合格と言われちゃって、早く合格して皆さんのお役に立ちたいんです。
//}
//talk[chara_maria]{
う〜ん、不合格って言い方は厳しいなぁ…。またムチさんに真意を聞いてみようかな。
//}
//}

ところがムチさんは席にいなかったので、マリアさんはアメさんのところに行き、経緯を説明しました。

//talklist{
//talk[chara_maria]{
ということがあったんですが、アメさんはどう思いますか？
//}
//talk[chara_ame]{
エマちゃんは既に私たちのメンバーですし、私は不合格などと評価したりしないですよ。
//}
//talk[chara_ame]{
エマちゃんは私からフォローしますね。
//}
//talk[chara_maria]{
いつもごめんね、アメさん。
//}
//}

//note[■試験形式のレビューとは]{
・レビューの最後に「合格」「不合格」みたいな用語が飛び交う

・タイプミスや改善点を見つけて指摘することを、失敗の通達と捉えている
//}

これは知識が浅い人やニューフェイスがいると起こりがちですが、エマちゃん、ムチさん、アメさん、マリアさんの4人は、立場こそ違えど「ソフトウェアを完成させ、ユーザーを満足させる」という目的は一緒なはずです。

その中で試験のようなレビューをすると、発表者はユーザーではなくレビュワーの満足度を高くすることに注力したり、試験に合格することが目的と錯覚します。そして試験に落ちるとやる気を損ない、協調的であるべきメンバーの和が、いつの間にか乱れてしまうのです。

//talklist{
//talk[chara_ame]{
エマちゃん、それおいしい？
//}
//talk[chara_ema]{
あ、これですか！この前落ち込んでるときコンビニで見かけた新作のしらたまあんぱんです！なんかホッとする味です！
//}
//talk[chara_ame]{
いいね。私も食べてみようかな。ところで、この前レビューしてた期限の近いあれ、どうなった？
//}
//talk[chara_ema]{
時間がなかったのでムチさんに続きを引き取ってもらいました…。
//}
//talk[chara_ema]{
えーん、思い出したら悲しくて、もう1個食べたくなっちゃいました〜。買ってきます〜。
//}
//talk[chara_ame]{
待って待って！あれね、実は私も途中まで見てコメントしかけてたの。これ見て。
//}
//}

//list[00001][＠Muchi commented]{
1. このAPIが例外を起こしたときの考慮
2. typoやインデントずれ
これらを直していただければ、残りはLGTMです。
//}

//talklist{
//talk[chara_ema]{
（LGTM…そうだったんだ。よかった…。間違えたところは、次は気をつけよう）
//}
//}

それから、エマちゃんのしらたまあんぱんブームも終わり、気になってた体重もそれほど増えずに済んだのでした。

アメさんがエマちゃんに見せたコメントは、評価などの不必要な言及を避け、そのとき必要な指摘だけを伝えるものでした。そして、やはり良かった部分は認めています。

何かを書いて校正を頼むのは試験ではなく、同じ目的を持つ仲間同士の協力作業なんです。

== 意識高い系

それから1ヶ月経ち、エマちゃんは困ったことは何でもアメさんに相談し、順調にスキルアップしていきました。

そして、エマちゃんは少し大きな実装を任されました。この間勉強したばかりのアーキテクチャーやデザインパターンを駆使し、わからないところはアメさんに質問しながら、なんとか仕上げました。

//talklist{
//talk[chara_ema]{
アメさん、色々とありがとうございました！
//}
//talk[chara_ame]{
いえいえ。早くムチさんからもApproveをもらえるといいね！
//}
//talk[chara_ema]{
（今回はアメさんの言う通りに作ったし、ムチさんも納得してくれるはず…！）
//}
//}

エマちゃんはルンルン気分でPRを作り、退社しました。水曜日だし、ウェンズデーを活用して前から見たかった映画を見たり、ケーキバイキングに行ったりして、オフをエンジョイしました。

翌日、出社するとムチさんの指摘が来ていました。

//list[00001][＠Muchi commented]{
この判定はビジネスロジックなので、サービスクラスでやる仕事じゃないです。もっとドメインを意識して書き直して下さい。
doTask()に2つ以上の責務があり意味不明です。
//}

//talklist{
//talk[chara_ema]{
(ぐす…アメさんに色々聞いて頑張ったのに書き直しだなんて…)
//}
//talk[chara_ame]{
エマちゃん、Approveもらえた？
//}
//talk[chara_ema]{
アメさーん！（泣）
//}
//talk[chara_ame]{
ど、どうしたの！？
//}
//}

アメさんは泣くエマちゃんを必死になだめました。その日の午後、アメさんがマリアさんに呼ばれました。

//talklist{
//talk[chara_maria]{
エマちゃんと1on1をしたんですが、ちょっと自信を失ってるようだね。
//}
//talk[chara_ame]{
はい、私がレビューしてOKだったんですが、ムチさんにとってはNGだったようで…。
//}
//talk[chara_maria]{
レビューのチェックポイントは2人とも同じなんだよね？なんで分かれたの？
//}
//talk[chara_ame]{
私はそこまで関心の分離を徹底しようと思ってなかったんですが、ムチさんは徹底したかったようです。
//}
//talk[chara_ame]{
それに、エマちゃんは何も考えてないわけじゃなく、エマちゃんなりの考えでこういう記述してるんだなって感じて、尊重してあげたかったんです。
//}
//talk[chara_ame]{
だから、今はOKを出して、高度な実装は追々学んでくれるといいな〜と思ったのが、私側の感覚ですかね。
//}
//talk[chara_maria]{
なるほどね。アメさんから見て、ムチさんのやり方はどう思う？
//}
//talk[chara_ame]{
厳しめに見てくれるのはありがたいんですが、頑張って書いたのに突き放すような言い方はよくないかなって思いますね。
//}
//}

//note[■意識高い系のレビューとは]{
・突き放す感じの指摘で、具体的な修正指示が含まれていない

・タイプミスや改善点を見つけて指摘することを、失敗の通達と捉えている
//}

どんな実装にも、レビュイーにとってはベストの実装だったり、レビュイーなりの考えがあることが多いので、否定だけはよくありません。具体的な修正指示や例を出してあげる方が親切です。

また、答えをそのまま教えればよいとも限りません。あくまで「例」を示し、レビュイーが自分で考え、答えはネットや本で見つけるほうが、スキルアップやモチベーションに繋がることがあります。

（ここで4コマ漫画を挿入したい。ストーリーは、マリアさんがみんなをランチに誘ったらムチさんが「大丈夫です」と断る。「大丈夫ですってなにー！？」ってツッコミが入る）

== 持久戦

さて、レビューを受けるのはエマちゃんだけではありません。

マリアさんとアメさんは、今後のプロジェクト計画についてプロジェクト責任者やステークホルダーたちと対面でのレビューを受けました。しかし、その会議は予定終了時刻を2時間もオーバーした上、いくつかの議論は行われませんでした。

//talklist{
//talk[chara_maria]{
遅くまでお疲れ様。
//}
//talk[chara_ame]{
あの方々とやると、いつも予定時間を超えますよね〜。
//}
//talk[chara_maria]{
ホントにね、やんなってくる。
//}
//talk[chara_ame]{
それに最初は段取り良かったけど、途中から同じ話の繰り返しでした。
//}
//talk[chara_maria]{
終わりのほうは、向こうも疲れてたしね。
//}
//talk[chara_ame]{
しかも結論がいくつか出てないと思うので、別の日に再レビュー設定することになりそうです。
//}
//talk[chara_maria]{
最初の1時間で切り上げてもそんなに変わんなかったよね。
//}
//talk[chara_ame]{
決めた予定だから最後までやりたかったんでしょうね。
//}
//talk[chara_maria]{
今頃向こうは、やった！って満足感だけ残ってるんじゃない？
//}
//}

実はこのレビュー、向こうも満足感ではなく、マリアさんと同じようなことを感じ、愚痴っていたのでした。

つまり、誰か特定の人の振る舞いが良くなかったわけではなく、お互いにこのまま続けると非効率とわかっていながら、引っ込みがつかなかったのです。

//note[■持久戦のレビューとは]{
・やたらと長く、予定を超過する

・最初はやる気があっても、時間が経つと疲れる

・発表者もレビュアーも、時間が経つにつれ不注意になるが、切り上げられない

・体力を吸われ、レビューの精度も低くなる

・充実感を伴って終わり、本質的な問題に気づかない
//}

とはいえ、対面での議論を途中で切り上げるのは、思い切りの要ることです。スケジュール調整も必要なので容易ではありません。

最初に「この場ではこれだけ話す！」「議題が逸れたらチャットツールやチケットなど、非同期な方法で続きを議論する」など、あらかじめ合意しておくことが重要でしょう。

== 行き当たりばったり

エマちゃんたちの会社は、360度評価が採用されています。今日はムチさんがメンバーからの評価フィードバックを受ける日です。

//talklist{
//talk[chara_maria]{
ムチさんのプロジェクト貢献度、コードの質の良さ、ドキュメントの更新頻度はおおむね高い点数でした。
//}
//talk[chara_muchi]{
はい。
//}
//talk[chara_maria]{
良い点として、ベースの知識の豊富や、解決力、実装の早さなどが挙げられました。
//}
//talk[chara_muchi]{
はい。
//}
//talk[chara_maria]{
一方、改善点として、円滑なコミュニケーションがややしづらいことや、他の方との交流が少なく、話しかけづらいなどがありました。
//}
//talk[chara_muchi]{
なるほど。
//}
//talk[chara_maria]{
改善点について、思うことはありますか？
//}
//talk[chara_muchi]{
特に…ないです。
//}
//talk[chara_maria]{
（ムチさん、毎回こうなんだよな〜。本心が読めない…）
//}
//}

席に戻ったムチさんは、さっさと帰り支度を始めました。

実はムチさん、帰りに書店でインテリア雑誌を立ち読みし、良い本なら買うのが日課です。今日も観葉植物を替えたくて新刊を読んでいますが、どうにも落ち着きません。360度評価をきっかけに、過去数ヶ月のことが気になり始めたようです。

//talklist{
//talk[chara_muchi]{
（今まで通りやってるはずなのに、新人が配属されてからなんかペースが狂うなぁ…）
//}
//talk[chara_muchi]{
（アメさんが人によって態度やレビューの許可基準を変えるから、僕がうまくフォローしてるつもりなんだけど）
//}
//talk[chara_muchi]{
（このままレビュー品質が担保されなくなる前に、PMに警告しとこうかな）
//}
//}

翌日、ムチさんはSlackでマリアさんに「プロジェクトに潜在リスクがあります」と一言送りました。何のことか見当つかなかったマリアさんは急いで戻り、ムチさんと直接話すことにしました。

//talklist{
//talk[chara_maria]{
…じゃ、リスクはバグとかのことじゃないんだね。
//}
//talk[chara_muchi]{
ええ。アメさんをレビュアーとして教育すべきだと思います。
//}
//talk[chara_maria]{
（ムチさんが、気になることを自分から話してくれたのは初めてだなぁ）
//}
//talk[chara_maria]{
（でも、エマちゃんのフォローとか頑張ってくれてるアメさんに、何か足りない点があるんだろうか…）
//}
//talk[chara_maria]{
アメさんのどのあたりが足りてないの？
//}
//talk[chara_muchi]{
レビューが行き当たりばったりなところかと。
//}
//talk[chara_maria]{
行き当たりばったり…か。私にはよくわかんないけど事実としてムチさんはそう感じてるってことだね？
//}
//talk[chara_muchi]{
はい。
//}
//talk[chara_maria]{
（もしかして、この前のOKとNGが2人の間で分かれた話が何か関係してるのかなぁ）
//}
//talk[chara_maria]{
わかった、アメさんにも意見聞いてみるね。
//}
//talk[chara_muchi]{
よろしくお願いします。
//}
//}

マリアさんは、後で行き当たりばったりとは何かを何度も考え直しました。一般的には、次のような状態を指します。

//note[■行き当たりばったりのレビューとは]{
・複数のレビュアーがおり、人それぞれレビューポイントや評価基準が大きく違う

・同じレビュアーでも一貫性がない。例えば、グローバル変数をある日は禁止し、別の日は許可する

・前と同じ問題が見つかっても、前と同じ指摘が行われない。言うのを躊躇う。同じ指摘は2度と行われない
//}

これらは人数が増えれば起こりがちです。ここで間違えないでほしいのが、「評価基準が"大きく"違う」と書いたように、多少は違ってよいのです。人間の目によるチェック漏れは0にできないし、レビュアーのスキルや経験に応じてレビューポイントが違うのも当然です。例えば可読性の低さやブラックボックス化の懸念など、不慣れな人のほうが気づきやすいこともありますね。

ですが、あまりにも行き当たりばったりな場合、発表者が気づいて理不尽さを感じたり、鬱憤、やる気の低下に繋がりかねません。

また、チェック漏れによる品質低下リスクも高まります。行きすぎないよう、Tryと改善が必要です。

== 独裁の場

//talklist{
//talk[chara_maria]{
アメさん、ちょっといいかな。
//}
//talk[chara_ame]{
はい、なんでしょう。
//}
//talk[chara_maria]{
ムチさんから、アメさんもムチさんと同じように厳しくレビューしてほしいと言われているんですが。
//}
//talk[chara_ame]{
ほぉ、そうなんですか。
//}
//talk[chara_maria]{
アメさんはどう思う？
//}
//talk[chara_ame]{
確かに、レビュー基準は合わせたいですよね。
//}
//talk[chara_maria]{
どんな風に合わせます？
//}
//talk[chara_ame]{
例えば、Google社の公開するCode Review Developer Guideをみんなで読み合わせたりとか。
//}
//talk[chara_maria]{
なるほど、じゃ、アメさんもムチさんに賛成なんだね。
//}
//talk[chara_ame]{
あ、でもムチさんと同じようにには簡単には乗れないですね。
//}
//talk[chara_ame]{
誰かを絶対正しいみたいにすると、独裁的なレビューになってよくないと思うんです。
//}
//talk[chara_maria]{
独裁的なレビュー？
//}
//}

//note[■独裁的なレビューとは]{
・誰か1人が権力を握り、レビューを支配している

・権力者に承認されることがレビューの目的となる
//}

誰かが権力者になると、権力者なしで意思決定できないチームになったり、開発者のキャリアアップを妨げ、チームの人数が増えても成果も上がらなくなる傾向があります。

//talklist{
//talk[chara_maria]{
なるほどね、じゃ続きはアメさんとムチさんで直接相談してもらえるかな？
//}
//talk[chara_ame]{
はい、そうします！
//}
//}

== 論点がズレている

//talklist{
//talk[chara_ame]{
ということで、ムチさん、よろしくお願いします！
//}
//talk[chara_muchi]{
はい。
//}
//talk[chara_ame]{
まず、私たちのレビューのやり方で、認識が合っている部分を明確にしません？
//}
//talk[chara_ame]{
私は重要じゃないチェックに時間をかけなくてよいと思ってます。それはムチさんも一緒なんじゃないかと思います。
//}
//talk[chara_muchi]{
重要じゃないとは具体的に？
//}
//talk[chara_ame]{
例えば命名規則とか、プラグインバージョン、スペルミス、マジックナンバーとかです。
//}
//talk[chara_muchi]{
そんなの人力で見てちゃダメだね。
//}
//}

//note[■論点がずれているレビューとは]{
・ソフトウェアの出荷には重要でない、コーディングの細かい部分ばかり言及される

・自動レビューで賄える箇所を、手動で頑張ってチェックする
//}

もちろんその洗い出し自体は大事ですが、そこばかり見すぎていると重要なものを見逃したり、時間を予想以上に費やしてしまいます。

これは几帳面な性格の人がいると起こりがちですが、人が見るレビューでは、ソフトウェアの品質に満足しているかとか、メンテナンス性、複数のやり方から最適な方法の選択といった、人でなければ判断できないことに集中しましょう。

//talklist{
//talk[chara_ame]{
では、ムチさんと私で認識が違ってる部分は、どのへんが気になりますか？
//}
//talk[chara_muchi]{
アメさんは新人に優しすぎたり、以前NGだったものもOKで返すことがあるよね？
//}
//talk[chara_ame]{
う〜ん…確かに状況に応じてスピードを上げたり、反対に性能や品質を優先したりしてますね。
//}
//talk[chara_muchi]{
では、その状況判断の根拠は何？
//}
//talk[chara_ame]{
えーと…正直、直感ですね。
//}
//talk[chara_muchi]{
それが良くないよね。レビューは責任持ってやってくれなきゃ。
//}
//talk[chara_ame]{
すみません、わかりました。今後はムチさんの指示通りにレビューします。
//}
//talk[chara_muchi]{
お願いします。
//}
//talk[chara_muchi]{
（またなんかペース狂う…）
//}
//talk[chara_maria]{
アメさん、ムチさん、大変です。市場トラブルです！
//}
//}

== 一方通行

マリアさんはプロジェクトメンバー全員を集め、今大規模な障害で全ユーザーがアプリを使えなくなっていること、至急修正が必要なことを伝えました。

エマちゃん、アメさん、ムチさんのチームは、大掛かりの修正をタスク分けしてスピーディーにこなしましたが、先ほどのレビューの話をしたばかりで時間がなく、レビュ〜はムチさんが全て引き受けました。

//list[00001][＠Muchi commented]{
@Ame
このユーティリティクラスを作った意図が伝わりません。
//}
//list[00001][＠Ame commented]{
今後他のモジュールが呼ぶ可能性があるので作りました！
//}
//list[00001][＠Muchi commented]{
それなら必要に応じてリファクタリングすれば良いと思います。
こんなイケてないことするくらいなら、今はコンパクトな参照関係にすべきです。
//}
//list[00001][＠Ame commented]{
わかりました、修正します！
//}

//talklist{
//talk[chara_ema]{
（なんか、アメさんがムチさんにしごかれる流れになってるけど…チーム大丈夫なのかな）
//}
//}

//list[00001][＠Muchi commented]{
@Ema
UIの変更が含まれてませんが、スケジュールはUI含め本日までです。レビュー都合もあるのでボヤボヤせず早めに出してください。
//}

//talklist{
//talk[chara_ema]{
（ひぃん、人の心配してる場合じゃなかったぁ…）
//}
//talk[chara_ema]{
（でも、ムチさんのコードって誰がレビューしてるんだろ…）
//}
//}

エマちゃんはメインブランチのマージログを確認しました。すると、ムチさんはレビューなしでMainブランチに入れていました。

//talklist{
//talk[chara_ema]{
アメさん！ムチさんのコードって誰もレビューしなくていいんですか？
//}
//talk[chara_ame]{
う〜ん、したほうがいいよね…でも今そんな余裕ないし、ムチさん一番先輩だからまぁ大丈夫かなって。
//}
//talk[chara_ema]{
なるほど、そういうものなんですね。
//}
//talk[chara_ame]{
あっ、ごめんねエマちゃん！そういうものではなく、見過ごしてただけでこれは本当はダメなパターンなの。
//}
//talk[chara_ema]{
えっ？
//}
//talk[chara_ame]{
一方通行のアンチパターンってよく呼んでるけど、余裕がなくてすっかり忘れてたわ。思い出させてくれてありがとう！
//}
//}

//note[■一方通行のレビューとは]{
・レビュアーのコードを誰もレビューしない

・上下関係があり、若手が先輩のコードをレビューしない

・一番上の人が自分のコードをアンレビューでマージする
//}

これは年功序列や経験差がハッキリしたチームで起こりがちです。実は経験が浅い人こそ、他の人に無い観点で問題に気づくことがあるのです。実際、みなさんのチームでも若手が思わぬ改善点を見つけくれることはよくありますよね。

ベテランの書くコードにも意外な問題が潜んでいるものです。厳しめに言うと、長くいる人は成長してスキルが優れているというより、単に仕事に慣れただけということもよくあります。

そういった互いの立場を理解して、足りないところを埋め合うほうが、チーム内の雰囲気も品質も良くなるのです。

//talklist{
//talk[chara_ame]{
だからエマちゃん、もし手が空いてたら私やムチさんのコードをレビューしてくれない？
//}
//talk[chara_ema]{
えっ、私なんかがいいんですか？
//}
//talk[chara_ame]{
エマちゃんしか気づけないこともたくさんあると思うの、だからお願い。ムチさんには私から説明しておくわ。
//}
//talk[chara_ema]{
わかりました、やります！
//}
//}

そして初めてレビュアーとなったエマちゃんでした。

//talklist{
//talk[chara_muchi]{
なぜエマさんがレビューをしているんですか？誰の指示ですか？
//}
//talk[chara_ame]{
私です、エマさんの成長と一方通行レビューにならないようにお願いしてます！
//}
//talk[chara_muchi]{
（また独断で…。新人にレビューのやり方教える前に自分が学んで欲しいんだけど…）
//}
//talk[chara_muchi]{
（ま、新人に細かい実装のこと教える手間が減るのならいいか）
//}
//}

エマちゃんは最初ほとんどApproveでしたが、やがて小さな部分に目が届くようになりました。それに、アメさんとも技術的な踏み込んだ話ができるようになりました。ムチさんはまだ怖いようですが…。

一方、アメさんとムチさんの間も変化しつつあります。ムチさんはアメさんに厳しくし始め、アメさんは半ば諦めたようにムチさんのやり方に従い始めました。アメさんには一体どんな考えがあるのでしょうか。次章に続きます。

== 良いレビューとは

レビューのアンチパターン集はここまでです。では、良いレビューとは一体何を満たすレビューでしょうか。筆者は、以下の要素を持つものと考えます。

1つめは、建設的なフィードバックがあり、健全で活発な議論を生むことです。コメントを書く人は、自分の考えを明確にしつつ、それが正しいものだとは主張しません。状況に応じていろんな代替案や回避策が存在することを認め、修正の強要ではなく、どう思う？などと問いかけます。もしレビュイーから反対意見が出た場合は、インターネットや先人の知恵に寄り道して、2人の間でいい着地点を探しましょう。それが難しそうなら、第3者に客観的な意見を求めるのがよいでしょう。

2つめは、レビュアーがレビュイーに共感していることです。いかなる方法でも、レビュイーが実装にかけた時間や労力を称賛し、強い口調ではなく親切で控えめな口調で接することが大切です。たくさんコメントした場合は、プルリクエスト以外の場所、例えば対面やオンライン会議、チャットやメールなどで積極的に連絡を取り、誤解が生じるのを防ぎます。そうすればきっとつらい思いをする人は減り、レビューの雰囲気や会話の方向性がとてもポジティブなものとなるでしょう。

3つめは、全ての人が対等であることです。職種や職歴、スキルレベル、入社時期、サラリーなどに影響されず、全員が同じ基準、同じ目線で、かつお互い足りないものを埋める場所としてレビューを活用します。アンチパターンに挙げたような、スキルテストや評価の場として利用するのは適切ではありません。
