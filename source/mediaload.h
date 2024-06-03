#ifndef MEDIALOAD_H
#define MEDIALOAD_H

#include <QWidget>
#include <QLabel>
#include <QLayout>
#include <QFileDialog>
#include <QDir>
#include <QMimeData>
#include <QDropEvent>
#include <QMouseEvent>

#include <betterfiles.hpp>

#ifndef MEDIALOAD_MACRO
#define MEDIALOAD_MACRO

#define IMAGE_FILTER "(*.png *.jpeg *.jpg *.bmp)"
#define VIDEO_FILTER "(*.mp4 *.av3 *.ts *.mov)"
#define MUSIC_FILTER "(*.mp3 *.ogg *.flac)"

#endif // !MEDIALOAD_MACRO

class BetterLabel : public QLabel
{
    Q_OBJECT

public:
    BetterLabel(QWidget *parent = nullptr) :
        QLabel(parent), mImage(QPixmap())
    {
        setAlignment(Qt::AlignCenter);
    };
    BetterLabel(const QString &text, QWidget *parent = nullptr) :
        QLabel(text, parent), mImage(QPixmap())
    {
        setAlignment(Qt::AlignCenter);
    };
    BetterLabel(const QPixmap &image, QWidget *parent = nullptr) :
        QLabel(parent), mImage(image)
    {
        setAlignment(Qt::AlignCenter);
        updateImage();
    };

    void setSize(const QSize &size, bool syncContent = true) {
        setMinimumSize(size);
        setMaximumSize(size);
        if (syncContent) {
            updateImage();
        }
    }
    void setSize(int w, int h, bool syncContent = true) {
        setMinimumSize(w, h);
        setMaximumSize(w, h);
        if (syncContent) {
            updateImage();
        }
    }
    void setImage(const QPixmap &image) {
        mImage = image;
        updateImage();
    }
    void setImage(const QImage &image) {
        mImage = QPixmap::fromImage(image);
        updateImage();
    }

signals:
    void clicked(Qt::MouseButton);
    void pressed(Qt::MouseButton);
    void released(Qt::MouseButton);

protected:
    void mousePressEvent(QMouseEvent *event) override {
        isPressed = true;
        mPressedButton = event->button();
        emit pressed(event->button());
    }
    void mouseReleaseEvent(QMouseEvent *event) override {
        if (isPressed == true &&
            event->button() == mPressedButton && rect().contains(event->pos()))
        {
            emit clicked(event->button());
        }
        isPressed = false;
        emit released(event->button());
    }

private:
    void updateImage() {
        if (mImage.isNull())
            return;
        setPixmap(mImage.scaled(size(), Qt::KeepAspectRatio));
    }

private:
    bool isPressed = false;
    Qt::MouseButton mPressedButton = Qt::MouseButton::AllButtons;
    QPixmap mImage;
};

class MediaLoad : public QWidget
{
    Q_OBJECT

public:
    enum Type : char
    {
        Image,
        Video,
        Music
    };

public:
    MediaLoad(Type type, QWidget *parent = nullptr, const QPixmap &image = QPixmap()) :
        QWidget(parent), mType(type), mShow(new BetterLabel(this)), 
        mPathShow(new BetterLabel(this)), mDefaultImage(image)
    {
        // Set the own attribute.
        setAcceptDrops(true);
        setLayout(new QVBoxLayout());
        layout()->setContentsMargins(QMargins(0, 0, 0, 0));
        layout()->setSpacing(0);

        // Add widgets to layout.
        layout()->addWidget(mPathShow);
        layout()->addWidget(mShow);

        // Set the member attribute.
        mPathShow->setAlignment(Qt::AlignCenter);
        mShow->setImage(mDefaultImage);

        // Connect signals and slots.
        connect(mShow, &BetterLabel::clicked, this, &MediaLoad::readResource);
    }
    ~MediaLoad() {};

    QString getFilePath() const {
        return mFilePath;
    }

    void setSize(const QSize &size, bool syncContent = true) {
        setMinimumSize(size);
        setMaximumSize(size);
        mShow->setSize(size, syncContent);
    }
    void setSize(int w, int h, bool syncContent = true) {
        setMinimumSize(w, h);
        setMaximumSize(w, h);
        mShow->setSize(w, h, syncContent);
    }
    void setDefaultImage(const QPixmap &image) {
        mDefaultImage = image;
     }
    void setPictureThumb(const QPixmap &image) {
        mPictureThumb = image;
    }
    void setVideoThumb(const QPixmap &image) {
        mVideoThumb = image;
    }
    void setMusicThumb(const QPixmap &image) {
        mMusicThumb = image;
    }
    void setDefaultPath(const QString &path) {
        mDefaultPath = path;
    }

    void reset() {
        mShow->setImage(mDefaultImage);
        clearPath();
    }

protected:
    void dragEnterEvent(QDragEnterEvent *event) override {
        if (event->mimeData()->hasUrls()) {
            event->acceptProposedAction();
        }
    }
    void dropEvent(QDropEvent *event) override {
        const QMimeData *mimeData = event->mimeData();
        if (!mimeData->hasUrls())
            return;

        QList<QUrl> urlList = mimeData->urls();
        setPath(urlList.at(0).toLocalFile());
        switch (mType) {
            case Image:
                loadImage();
                break;
            case Video:
                loadVideo();
                break;
            case Music:
                loadMusic();
                break;
            default:
                qDebug() << __FILE__ << __LINE__ << __FUNCTION__ << "Invaild type.";
                break;
        }
    }

private:
    void clearPath() {
        mFilePath.clear();
        mPathShow->setText(QString());
    }

    void setPath(const QString &path) {
        mFilePath = path;
        mPathShow->setText(path);
    }

    void loadImage() {
        QPixmap image(mFilePath);
        if (image.isNull()) {
            // TODO Popup
            reset();
            return;
        }
        if (mPictureThumb.isNull())
            mShow->setImage(image);
        else
            mShow->setImage(mPictureThumb);
    }
    void loadVideo() {
        if (!Bf::isExistsFile(mFilePath.toStdString())) {
            reset();
            return;
        }
        if (mVideoThumb.isNull())
            mShow->setImage(QPixmap());
        else
            mShow->setImage(mVideoThumb);
    }
    void loadMusic() {
        if (!Bf::isExistsFile(mFilePath.toStdString())) {
            reset();
            return;
        }
        if (mMusicThumb.isNull())
            mShow->setImage(QPixmap());
        else
            mShow->setImage(mMusicThumb);
    }
    void readResource() {
        QString currentPath;
        if (mDefaultPath.isEmpty())
            currentPath = QDir::currentPath();
        else
            currentPath = mDefaultPath;

        switch (mType) {
            case Image:
                setPath(QFileDialog::getOpenFileName(this, tr("Open File"), currentPath,
                                                     tr("Image File") + IMAGE_FILTER));
                loadImage();
                break;
            case Video:
                setPath(QFileDialog::getOpenFileName(this, tr("Open File"), currentPath,
                                                     tr("Video File") + VIDEO_FILTER));
                loadVideo();
                break;
            case Music:
                setPath(QFileDialog::getOpenFileName(this, tr("Open File"), currentPath,
                                                     tr("Music File") + MUSIC_FILTER));
                loadMusic();
                break;
            default:
                qDebug() << __FILE__ << __LINE__ << __FUNCTION__ << "Invaild type.";
                break;
        }
    }

private:
    Type mType;
    BetterLabel *mShow;
    BetterLabel *mPathShow;
    QString mDefaultPath;
    QString mFilePath;
    QPixmap mDefaultImage;
    QPixmap mPictureThumb;
    QPixmap mVideoThumb;
    QPixmap mMusicThumb;
};

#endif // !MEDIALOAD_H
