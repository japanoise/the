EAPI=6

DESCRIPTION="The Hessling Editor: Clone of IBM's XEDIT editor"
HOMEPAGE="http://hessling-editor.sourceforge.net/"
SRC_URI="https://github.com/japanoise/the/releases/download/v3.3-RC4/THE-3.3RC4.tar.gz"

LICENSE="GPL−2"
SLOT="0"
KEYWORDS="~x86 ~amd64"
IUSE=""

DEPEND="dev-lang/regina-rexx sys-libs/ncurses"
RDEPEND="${DEPEND}"

src_unpack() {
	unpack ${A}
	mv THE-3.3RC4 the-3.3.4
}

src_configure() {
	econf \
	--with-ncurses \
	--with-rexx=regina
	--enable-utf8
}
